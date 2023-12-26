use crate::common::{
    args::CommonArgs,
    utils::{self, PushField},
    wire,
};
use anyhow::Result;
use serde::{Deserialize, Serialize};

pub async fn show(common: CommonArgs, ip6: bool) -> Result<()> {
    fn no_v(session: &Session) -> &str {
        let addr = session.addr.as_str();
        addr.split_once('.').map(|(_, s)| s).unwrap_or(addr)
    }

    let mut cjdns = cjdns_admin::connect(Some(common.as_anon())).await?;
    let mut handles = vec![];
    let mut page = 0;
    loop {
        let mut resp: Handles = cjdns
            .invoke("SessionManager_getHandles", HandlesArgs { page })
            .await?;
        handles.append(&mut resp.handles);
        if resp.more {
            page += 1;
        } else {
            break;
        }
    }

    let mut sessions = vec![];
    for handle in handles {
        let resp: Session = cjdns
            .invoke("SessionManager_sessionStats", SessionArgs { handle })
            .await?;
        sessions.push(resp);
    }
    sessions.sort_by(|a, b| no_v(a).cmp(no_v(b)));

    let mut lines = Vec::with_capacity(sessions.len());
    for session in sessions {
        let addr = if ip6 {
            utils::key_to_ip6(&session.addr, true)?
        } else {
            session.addr
        };
        let mut last = String::new();
        if session.duplicates > 0 {
            last.push_field(format!("DUP {}", session.duplicates));
        }
        if session.lost_packets > 0 {
            last.push_field(format!("LOS {}", session.lost_packets));
        }
        if session.received_out_of_range > 0 {
            last.push_field(format!("OOR {}", session.received_out_of_range));
        }
        lines.push([
            addr,
            session.state,
            session.handle.to_string(),
            session.send_handle.to_string(),
            format!("{:08x}", session.metric),
            last,
        ]);
    }
    utils::print_padded(lines);
    Ok(())
}

#[derive(Serialize)]
struct HandlesArgs {
    page: u32,
}

#[derive(Deserialize)]
struct Handles {
    handles: Vec<u32>,
    #[serde(deserialize_with = "wire::as_bool", default)]
    more: bool,
}

#[derive(Serialize)]
struct SessionArgs {
    handle: u32,
}

#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
struct Session {
    addr: String,
    duplicates: u32,
    handle: u32,
    lost_packets: u64,
    metric: u64,
    received_out_of_range: u64,
    send_handle: u32,
    state: String,
}
