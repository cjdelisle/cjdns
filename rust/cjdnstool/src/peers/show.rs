use crate::common::{
    args::CommonArgs,
    utils::{self, PushField},
    wire,
};
use eyre::Result;
use serde::{Deserialize, Serialize};

pub async fn show(common: CommonArgs, ip6: bool) -> Result<()> {
    let mut cjdns = cjdns::admin::connect(Some(common.as_anon())).await?;
    let mut lines = vec![];
    let mut page = 0;
    loop {
        let resp: Peers = cjdns
            .invoke("InterfaceController_peerStats", Args { page })
            .await?;
        for peer in resp.peers {
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

        if resp.more {
            page += 1;
        } else {
            break;
        }
    }

    utils::print_padded(lines);

    Ok(())
}

#[derive(Serialize)]
struct Args {
    page: u32,
}

#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
struct Peer {
    addr: String,
    duplicates: u32,
    lladdr: String,
    lost_packets: u64,
    received_out_of_range: u64,
    recv_kbps: u32,
    send_kbps: u32,
    state: String,
    #[serde(default)]
    user: String,
}

#[derive(Deserialize)]
struct Peers {
    #[serde(deserialize_with = "wire::as_bool", default)]
    more: bool,
    peers: Vec<Peer>,
}
