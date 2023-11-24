use crate::{
    common::CommonArgs,
    util::{self, PushField},
    wire,
};
use anyhow::{anyhow, Result};
use serde::{Deserialize, Serialize};

pub async fn show(common: CommonArgs, ip6: bool) -> Result<()> {
    let mut cjdns = cjdns_admin::connect(Some(common.as_anon())).await?;
    let mut lines = vec![];
    let mut page = 0;
    loop {
        let resp = cjdns
            .invoke::<_, Option<Peers>>("InterfaceController_peerStats", Args { page })
            .await?
            .ok_or_else(|| anyhow!("InterfaceController_peerStats: missing payload"))?;
        for peer in resp.peers {
            let addr = if ip6 {
                util::key_to_ip6(&peer.addr)?
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
            if let Some(user) = peer.user {
                last.push_field(format!("\"{user}\""));
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

        if resp.more {
            page += 1;
        } else {
            break;
        }
    }

    util::print_padded(lines);

    Ok(())
}

#[derive(Serialize)]
struct Args {
    page: u32,
}

#[allow(dead_code)]
#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
struct Peer {
    addr: String,
    bytes_in: u64,
    bytes_out: u64,
    duplicates: u32,
    if_num: u32,
    #[serde(deserialize_with = "wire::as_bool")]
    is_incoming: bool,
    last: u64,
    lladdr: String,
    lost_packets: u64,
    noise_proto: u32,
    received_out_of_range: u64,
    received_packets: u64,
    recv_kbps: u32,
    send_kbps: u32,
    state: String,
    #[serde(default)]
    user: Option<String>,
}

#[derive(Deserialize)]
struct Peers {
    #[serde(deserialize_with = "wire::as_bool", default)]
    more: bool,
    peers: Vec<Peer>,
}
