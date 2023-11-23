use crate::{common::CommonArgs, wire};
use anyhow::Result;
use serde::{Deserialize, Serialize};

pub async fn show(common: CommonArgs) -> Result<()> {
    let mut cjdns = cjdns_admin::connect(Some(common.as_anon())).await?;
    let mut peers = vec![];
    let mut page = 0;
    loop {
        let resp: Response = cjdns
            .invoke("InterfaceController_peerStats", Args { page })
            .await?;
        for peer in resp.peers {
            let mut info = format!(
                "{} {} in {} kb/s out {} kb/s",
                peer.addr, peer.state, peer.recv_kbps, peer.send_kbps
            );
            if peer.duplicates != 0 {
                info.push_str(&format!("  DUP {}", peer.duplicates));
            }
            if peer.lost_packets != 0 {
                info.push_str(&format!("  LOS {}", peer.lost_packets));
            }
            if peer.received_out_of_range != 0 {
                info.push_str(&format!("  OOR {}", peer.received_out_of_range));
            }
            if let Some(ref user) = peer.user {
                info.push_str(" \"");
                info.push_str(user);
                info.push('"');
            }
            peers.push((peer.lladdr, info));
        }

        if resp.more {
            page += 1;
        } else {
            break;
        }
    }

    if let Some(pad) = peers.iter().map(|(ref lladdr, _)| lladdr.len()).max() {
        for (lladdr, info) in peers {
            println!("{lladdr:pad$} {info}");
        }
    }

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

#[allow(dead_code)]
#[derive(Default, Deserialize)]
#[serde(rename_all = "camelCase")]
struct Response {
    #[serde(deserialize_with = "wire::as_bool", default)]
    more: bool,
    peers: Vec<Peer>,
    total: u32,
}
