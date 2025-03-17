use std::{net::Ipv6Addr, time::Duration};

use crate::{
    common::args::CommonArgs,
    route::ResolveFrom,
};
use eyre::{bail, Result, Context};
use cjdns::{
    bencode::object::{Dict, Get}, bytes::message::Message, core::{Address, DefaultRoutingLabel}, keys::{CJDNSPublicKey, CJDNS_IP6}, sniff::Connection
};
use clap::ValueEnum;
use rand::RngCore;

#[derive(Debug, Clone, PartialEq, ValueEnum)]
#[value(rename_all = "lower")]
pub enum Type {
    Router,
    Switch,
    Getkey,
    Lladdr,
    Rpath,
    Snode,
}

#[derive(Debug, Clone, PartialEq, ValueEnum)]
#[value(rename_all = "lower")]
pub enum Resolve {
    #[value(aliases = ["s"])]
    Session,
    #[value(aliases = ["n"])]
    Snode,
    #[value(aliases = ["d"])]
    Default,
    #[value(aliases = ["0"])]
    No,
}

pub const LONG_ABOUT: &str = r#"Send a cjdns ping to a node

Cjdns pings come in two varieties, a router ping and a switch ping.
A router ping is a normal inter-router query without any specific
payload, and a switch ping is a special (unencrypted) message which
is handled at a lower level.

The default type of ping to send is a route ping, all other types
are varients of switch ping.

The destination may be specified in one of three ways:

- Path only, e.g.
    cjdnstool ping 0000.0000.0260.0027
    Only valid for switch pings
- Full address, e.g.
    cjdnstool ping v21.0000.0000.0260.0027.znrj10d4s005qyg2wg2sx54k4yrqjxyvkc1pz4vqfzxdv5sxvds0.k
    Valid for any ping type, no resolution is performed
- IPv6 address, e.g.
    cjdnstool ping fc7f:fdb2:e104:39d7:ed74:446a:3c9f:6b4e
    Valid for any ping type, path resolution must be performed

Resolution can be performed in one of four ways:

- {session | s} check for an active communication session with the destination
    if one is not present then fail.
- {snode | n} query the supernode for a path to the destination
- {default | d} check for a session first then fallback to the snode
- {no | 0} refuse to resolve, error out if resolution is required

"#;

pub async fn router_ping(
    cjdns: &mut Connection,
    addr: &Address,
    data: Vec<u8>,
    count: u32,
    verbose: bool,
) -> Result<()> {
    let ip6 = CJDNS_IP6::try_from(&addr.pubkey).context("Invalid address")?;
    let ip6str = ip6.to_string();
    let astr = addr.to_string();
    for i in 0..count {
        let mut nargs = Dict::new();
        nargs.insert("q", "pn");
        if !data.is_empty() {
            nargs.insert("txid", &data[..]);
        }
        let mut args = Dict::new();
        args.insert("address", &astr);
        args.insert("args", nargs);
        // println!("Query Node");
        let res = match cjdns.invoke("SubnodePathfinder_queryNode", args).await {
            Ok(res) => res,
            Err(e) => {
                if e.to_string().contains("Timeout") {
                    println!("Timeout sending ping");
                    tokio::time::sleep(Duration::from_secs(1)).await;
                    continue;
                } else {
                    return Err(e.into());
                }
            }
        };

        if verbose {
            let mut msg = Message::new();
            cjdns::bencode::json::serialize(&mut msg, &res.clone().obj())?;
            println!("{}", String::from_utf8_lossy(&msg.as_vec()));
        }

        let error = res.get_str("error")?;
        if error != "none" {
            println!("Error sending ping: {error}");
        } else {
            let resp = res.get_dict("response")?;
            let txid = resp.get_bytes("txid")?;
            if txid != data {
                println!("Payload mismatch: sent: 0x{}, got 0x{}", hex::encode(&data), hex::encode(txid));
            }
            let address = res.get_str("address")?;
            if address != astr {
                println!("Address mismatch: sent: {astr}, got {address}");
            }
            let lag = res.get_int("lag")?;
            println!("{} bytes from {} ({}): seq={i} time={} ms", txid.len(), ip6str, address, lag);
        }

        tokio::time::sleep(Duration::from_secs(1)).await;
    }
    Ok(())
}

pub async fn switch_ping(
    cjdns: &mut Connection,
    args: Dict<'_>,
    count: u32,
    verbose: bool,
) -> Result<()> {
    for i in 0..count {
        if i != 0 {
            tokio::time::sleep(Duration::from_secs(1)).await;
        }
        let res = cjdns.invoke("SwitchPinger_ping", args.clone()).await?;
        if verbose {
            let mut msg = Message::new();
            cjdns::bencode::json::serialize(&mut msg, &res.clone().obj())?;
            println!("{}", String::from_utf8_lossy(&msg.as_vec()));
        }
        if let Some(error) = res.try_get_str("error")? {
            println!("Error sending ping: {error}");
            continue;
        }
        let result = res.get_str("result")?;
        let ms = res.get_int("ms")?;
        if result == "timeout" {
            println!("timed out after {} ms", ms);
            continue;
        }
        if result != "pong" {
            println!("Unknown result: {result}");
        }
        let bytes = if let Some(data) = args.try_get_bytes("data")? {
            if let Some(resdata) = res.try_get_bytes("data")? {
                if data != resdata {
                    println!("Payload mismatch: sent: 0x{}, got 0x{}",
                        hex::encode(resdata), hex::encode(data));
                }
            } else {
                println!("Didn't receive back pattern with ping");
            }
            data.len()
        } else {
            0
        };
        let path = res.get_str("path")?;
        if path != args.get_str("path")? {
            println!("Path mismatch: sent: {}, got {}", args.get_str("path")?, path);
        }
        if let Some(key) = res.try_get_str("key")? {
            let version = res.get_int("version")?;
            let pubkey = CJDNSPublicKey::try_from(key).context("Invalid cjdns key")?;
            let label = DefaultRoutingLabel::try_from(path).context("Invalid path")?;
            let ip6 = CJDNS_IP6::try_from(&pubkey).context("Invalid key")?;
            let addr = Address { pubkey, label, version: version as _ };
            println!("{} bytes from {} ({}): seq={} time={} ms", bytes, ip6.to_string(), addr.to_string(), i, ms);
        } else {
            println!("{} bytes from {}:{}{}{} seq={} time={} ms",
                bytes,
                path,
                if let Some(snode) = res.try_get_str("snode")? { format!(" snode={snode}") } else { "".into() },
                if let Some(rpath) = res.try_get_str("rpath")? { format!(" rpath={rpath}") } else { "".into() },
                if let Some(lladdr) = res.try_get_str("lladdr")? { format!(" lladdr={lladdr}") } else { "".into() },
                i,
                ms,
            );
        }
    }
    Ok(())
}

pub async fn ping(
    common: CommonArgs,
    typ: Option<Type>,
    resolve: Option<Resolve>,
    count: Option<u32>,
    length: Option<u16>,
    pattern: Option<String>,
    verbose: bool,
    dest: String,
) -> Result<()> {
    let mut cjdns = cjdns::admin::connect(Some(common.with_auth())).await?;

    let resolve = resolve.unwrap_or(Resolve::Default);
    let count = count.unwrap_or(u32::MAX);
    let pattern = {
        if let Some(pattern) = pattern {
            let pattern = hex::decode(pattern).context("Invalid hex pattern")?;
            if let Some(length) = length {
                if pattern.len() > length as usize {
                    bail!("pattern length {} is longer than length: {}", pattern.len(), length);
                }
                let mut v = Vec::new();
                while v.len() < length as usize {
                    v.extend_from_slice(&pattern);
                }
                v.truncate(length as usize);
                v
            } else {
                pattern
            }
        } else if let Some(length) = length {
            let mut v = vec![0_u8; length as usize];
            rand::thread_rng().fill_bytes(&mut v);
            v
        } else {
            Vec::new()
        }
    };

    let (addr, path, from_ip6) = if let Ok(a) = Address::try_from(&dest[..]) {
        let label = a.label;
        (Some((a,"provided")), Some(label), false)
    } else if dest.parse::<Ipv6Addr>().is_ok() {
        let from = match resolve {
            Resolve::No => bail!("Cannot resolve IPv6 address, --resolve=no"),
            Resolve::Default => None,
            Resolve::Session => Some(ResolveFrom::Session),
            Resolve::Snode => Some(ResolveFrom::Snode),
        };
        if let Some(r) = crate::route::resolve(&mut cjdns, &dest, None, from).await? {
            let label = r.full_addr.label;
            (Some((r.full_addr, r.src)), Some(label), true)
        } else {
            bail!("No route found");
        }
    } else if let Ok(path) = DefaultRoutingLabel::try_from(&dest[..]) {
        (None, Some(path), false)
    } else {
        bail!("Invalid destination: {}", dest);
    };

    let typ = typ.unwrap_or({
        if addr.is_none() {
            Type::Getkey
        } else {
            Type::Router
        }
    });

    match typ {
        Type::Router => {
            let Some(addr) = addr else {
                bail!("Router ping requires a full address");
            };
            let (addr, src) = addr;
            if from_ip6 {
                println!("PING {dest} ({}) {} bytes of data", addr.to_string(), pattern.len());
                println!("Address resolved from: {src}");
            } else {
                println!("PING {} {} bytes of data", addr.to_string(), pattern.len());
            }
            return router_ping(&mut cjdns, &addr, pattern, count, verbose).await;
        }
        _ => {
            let Some(path) = path else {
                bail!("No known path, cannot switch ping");
            };
            let mut args = Dict::new();
            args.insert("path", path.to_string());
            // cjdnstool cexec SwitchPinger_ping --path=<String> [--data=<String>] [--keyPing=<Int>] [--lladdr=<Int>] [--rpath=<Int>] [--snode=<Int>] [--timeout=<Int>]
            match typ {
                Type::Getkey => args.insert("keyPing", 1),
                Type::Lladdr => args.insert("lladdr", 1),
                Type::Rpath => args.insert("rpath", 1),
                Type::Snode => args.insert("snode", 1),
                _ => (),
            }
            if !pattern.is_empty() {
                args.insert("data", pattern);
            }
            return switch_ping(&mut cjdns, args, count, verbose).await;
        }
    }
}