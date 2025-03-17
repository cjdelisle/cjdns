use std::{net::Ipv6Addr, str::FromStr};

use crate::{
    common::args::CommonArgs,
    session::util::{print_metric, SNODE_SAYS},
};
use cjdns::{admin::Connection, core::{Address, DefaultRoutingLabel}, keys::{CJDNSPublicKey, CJDNS_IP6}};
use cjdns::bencode::object::{Dict,Get};
use eyre::{bail, eyre, Context, OptionExt, Result};

use super::ResolveFrom;

pub struct Route {
    pub full_addr: Address,
    pub metric: String,
    pub src: &'static str,
}

pub async fn get_snode(cjdns: &mut Connection) -> Result<Option<String>> {
    let ret = cjdns.invoke("SupernodeHunter_status", Dict::new()).await?;
    ret.try_get_string("activeSnode")
}

pub async fn get_self_addr(cjdns: &mut Connection) -> Result<String> {
    let ret = cjdns.invoke("Core_nodeInfo", Dict::new()).await?;
    ret.get_string("myIp6")
}

pub async fn route_from_session(cjdns: &mut Connection, ip6: &str) -> Result<Option<Route>> {
    let mut args = Dict::new();
    args.insert("ip6", ip6);
    let resp = match cjdns.invoke("SessionManager_sessionStatsByIP", args).await {
        Ok(resp) => resp,
        Err(e) => {
            if e.to_string().contains("no such session") {
                return Ok(None);
            } else {
                return Err(e.into());
            }
        }
    };
    let addr = resp.get_str("addr")?;
    let metric = resp.get_int("metric")?;

    Ok(Some(Route{
        full_addr: Address::try_from(addr)?,
        metric: print_metric(metric),
        src: "session",
    }))
}

pub async fn route_from_snode(cjdns: &mut Connection, ip6: &str, src: Option<String>) -> Result<Option<Route>> {
    let Some(snode) = get_snode(cjdns).await? else {
        bail!("No active supernode found");
    };

    let src = if let Some(src) = src {
        src
    } else {
        get_self_addr(cjdns).await?
    };

    let dst = Ipv6Addr::from_str(ip6).map_err(|_| eyre!("Invalid dest IPv6 address"))?.octets();
    let src = Ipv6Addr::from_str(&src).map_err(|_| eyre!("Invalid src IPv6 address"))?.octets();

    let mut sargs = Dict::new();
    sargs.insert("q","sq");
    sargs.insert("sq","gr");
    sargs.insert("src",&src[..]);
    sargs.insert("tar",&dst[..]);

    let mut args = Dict::new();
    args.insert("address", snode);
    args.insert("args", sargs);

    let resp = cjdns.invoke("SubnodePathfinder_queryNode", args).await?;

    let sres = resp.get_dict("response")?;
    if !sres.has("recvTime") {
        bail!("Snode did not respond");
    }
    let n = sres.get_bytes("n")?;
    let np = sres.get_bytes("np")?;
    if np.len() != 2 {
        bail!("Invalid np length from snode: {}", np.len());
    } else if np[0] != 1 {
        bail!("Unhandled np[0] from snode: {}", np[0]);
    }
    let version = np[1];

    let path = &n[32..];

    let k = CJDNSPublicKey::from({
        let mut key = [0_u8; 32];
        key.copy_from_slice(&n[..32]);
        key
    });
    let ip6 = CJDNS_IP6::try_from(&k)
        .context("Bad response from snode: computed IPv6 does not start with fc")?;
    if &ip6[..] != dst {
        bail!("Snode returned wrong IP6: {}", ip6);
    }

    let mut b = [0_u8; 8];
    b.copy_from_slice(&path[..8]);
    let p = u64::from_be_bytes(b);
    let path = DefaultRoutingLabel::try_new(p)
        .ok_or_eyre("Snode provided route is zero")?;

    let addr = Address{
        label: path,
        pubkey: k,
        version: version as u16,
    };

    Ok(Some(Route{
        full_addr: addr,
        metric: print_metric(SNODE_SAYS as _),
        src: "snode",
    }))
}

pub async fn resolve(
    cjdns: &mut Connection,
    dest: &str,
    src: Option<String>,
    from: Option<ResolveFrom>,
) -> Result<Option<Route>> {
    let r = match from {
        None | Some(ResolveFrom::Session) => route_from_session(cjdns, dest).await?,
        _ => None,
    };

    let r = match (r, from) {
        (Some(r), _) => Some(r),
        (None, None | Some(ResolveFrom::Snode)) => {
            route_from_snode(cjdns, dest, src).await?
        }
        _ => None,
    };

    Ok(r)
}

pub async fn get(common: CommonArgs, dest: String, src: Option<String>, from: Option<ResolveFrom>) -> Result<()> {
    let mut cjdns = cjdns::admin::connect(Some(common.as_anon())).await?;

    if let Some(r) = resolve(&mut cjdns, &dest, src, from).await? {
        println!("{} {}", r.full_addr.to_string(), r.src);
        Ok(())
    } else {
        bail!("No route found");
    }
}