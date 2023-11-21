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
    is_incoming: u32,
    last: u64,
    lladdr: String,
    lost_packets: u64,
    noise_proto: u32,
    received_out_of_range: u64,
    received_packets: u64,
    recv_kbps: u32,
    send_kbps: u32,
    state: String,
    user: String,
}

#[allow(dead_code)]
#[derive(Debug, Default, Deserialize)]
#[serde(rename_all = "camelCase")]
struct Response {
    peers: Vec<Peer>,
    total: u32,
}

pub async fn peer_stats(common: CommonArgs, page: u32) -> Result<()> {
    let mut cjdns = cjdns_admin::connect(Some(common.as_anon())).await?;
    let resp: Response = cjdns
        .invoke("InterfaceController_peerStats", Args { page })
        .await?;
    println!("{resp:?}");
    Ok(())
}
