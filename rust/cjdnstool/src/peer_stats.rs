use crate::common::CommonArgs;
use anyhow::Result;
use serde::{Deserialize, Serialize};

#[derive(Serialize)]
struct Args {
    page: u32,
}

#[allow(dead_code)]
#[derive(Debug, Deserialize)]
#[serde(rename_all = "camelCase")]
struct Peer {
    addr: String,
    bytes_in: u64,
    bytes_out: u64,
    duplicates: u32,
    if_num: u32,
    #[serde(deserialize_with = "crate::common::de_as_bool")]
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
#[derive(Debug, Default, Deserialize)]
#[serde(rename_all = "camelCase")]
struct Response {
    #[serde(deserialize_with = "crate::common::de_as_bool", default)]
    more: bool,
    peers: Vec<Peer>,
    total: u32,
}

pub async fn peer_stats(common: CommonArgs) -> Result<()> {
    let mut cjdns = cjdns_admin::connect(Some(common.as_anon())).await?;
    let mut page = 0;
    loop {
        let resp: Response = cjdns
            .invoke("InterfaceController_peerStats", Args { page })
            .await?;

        for peer in &resp.peers {
            let mut out = format!(
                "{} {} {} in {} kb/s out {} kb/s",
                peer.lladdr, peer.addr, peer.state, peer.recv_kbps, peer.send_kbps
            );
            if peer.duplicates != 0 {
                out.push_str(&format!("  DUP {}", peer.duplicates));
            }
            if peer.lost_packets != 0 {
                out.push_str(&format!("  LOS {}", peer.lost_packets));
            }
            if peer.received_out_of_range != 0 {
                out.push_str(&format!("  OOR {}", peer.received_out_of_range));
            }
            if let Some(ref user) = peer.user {
                out.push_str(" \"");
                out.push_str(user);
                out.push('"');
            }
            println!("{out}");
        }

        if resp.more {
            page += 1;
        } else {
            break;
        }
    }
    Ok(())
}
