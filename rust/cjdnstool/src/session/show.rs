use crate::{
    common::{
        args::CommonArgs,
        utils::{self, PushField},
    },
    session::util::print_metric,
};
use cjdns::bencode::object::{Dict,Get};
use eyre::Result;

pub async fn show(common: CommonArgs, ip6: bool) -> Result<()> {
    fn no_v(session: &Session) -> &str {
        let addr = session.addr.as_str();
        addr.split_once('.').map(|(_, s)| s).unwrap_or(addr)
    }

    let mut cjdns = cjdns::admin::connect(Some(common.as_anon())).await?;
    let mut handles: Vec<u32> = Vec::new();
    let mut page = 0;
    loop {
        let mut args = Dict::new();
        args.insert("page", page);
        let resp = cjdns
            .invoke("SessionManager_getHandles", args)
            .await?;
        for handle in resp.get_list("handles")?.iter() {
            handles.push(handle.try_into()?);
        }
        if resp.has("more") {
            page += 1;
        } else {
            break;
        }
    }

    let mut sessions: Vec<Session> = Vec::new();
    for handle in handles {
        let mut args = Dict::new();
        args.insert("handle", handle);
        let resp = cjdns.invoke("SessionManager_sessionStats", args).await?;
        sessions.push(resp.try_into()?);
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
            print_metric(session.metric),
            last,
        ]);
    }
    utils::print_padded(lines);
    Ok(())
}

struct Session {
    addr: String,
    duplicates: u32,
    handle: u32,
    lost_packets: i64,
    metric: i64,
    received_out_of_range: i64,
    send_handle: u32,
    state: String,
}
impl TryFrom<Dict<'_>> for Session {
    type Error = eyre::Error;

    fn try_from(value: Dict) -> Result<Self> {
        Ok(Session {
            addr: value.get("addr")?,
            duplicates: value.get("duplicates")?,
            handle: value.get("handle")?,
            lost_packets: value.get("lostPackets")?,
            metric: value.get("metric")?,
            received_out_of_range: value.get("receivedOutOfRange")?,
            send_handle: value.get("sendHandle")?,
            state: value.get("state")?,
        })
    }
}
