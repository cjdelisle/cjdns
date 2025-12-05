use crate::common::{
    args::CommonArgs,
    utils::{self, PushField},
};
use eyre::Result;
use serde::Deserialize;
use cjdns::bencode::object::{Dict,Get};

pub async fn show(common: CommonArgs, ip6: bool) -> Result<()> {
    let mut cjdns = cjdns::admin::connect(Some(common.as_anon())).await?;
    let mut lines = vec![];
    let mut page = 0;
    loop {
        let mut args = Dict::new();
        args.insert("page", page);
        let resp = cjdns
            .invoke("InterfaceController_peerStats", args)
            .await?;
        for peer in resp.get_list("peers")?.iter() {
            let peer: Peer = peer.as_dict()?.try_into()?;
            let addr = if ip6 {
                utils::key_to_ip6(&peer.addr, true)?
            } else {
                peer.addr
            };
            let mut last = String::new();
            if peer.duplicates != 0 {
                last.push_field(format!("DUP {}", peer.duplicates));
            }
            if peer.lost_packets != 0 {
                last.push_field(format!("LOS {}", peer.lost_packets));
            }
            if peer.received_out_of_range != 0 {
                last.push_field(format!("OOR {}", peer.received_out_of_range));
            }
            if !peer.user.is_empty() {
                last.push_field(format!("\"{}\"", peer.user));
            }
            lines.push([
                peer.lladdr,
                addr,
                peer.state,
                format!("in {}kb/s", peer.recv_kbps),
                format!("out {}kb/s", peer.send_kbps),
                last,
            ]);
        }

        if resp.has("more") {
            page += 1;
        } else {
            break;
        }
    }

    utils::print_padded(lines);

    Ok(())
}

#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
struct Peer {
    addr: String,
    duplicates: u32,
    lladdr: String,
    lost_packets: i64,
    received_out_of_range: i64,
    recv_kbps: u32,
    send_kbps: u32,
    state: String,
    #[serde(default)]
    user: String,
}
impl TryFrom<&Dict<'_>> for Peer {
    type Error = eyre::Error;

    fn try_from(dict: &Dict<'_>) -> Result<Self> {
        Ok(Self {
            addr: dict.get("addr")?,
            duplicates: dict.get("duplicates")?,
            lladdr: dict.get("lladdr")?,
            lost_packets: dict.get("lostPackets")?,
            received_out_of_range: dict.get("receivedOutOfRange")?,
            recv_kbps: dict.get("recvKbps")?,
            send_kbps: dict.get("sendKbps")?,
            state: dict.get("state")?,
            user: dict.try_get("user")?.unwrap_or_default(),
        })
    }
}