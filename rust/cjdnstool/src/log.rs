use std::time::Duration;

use chrono::{DateTime, SecondsFormat};
use cjdns::{
    admin::{cjdns_invoke, cjdns_invoke_subscribe},
    bencode::object::{Dict, Get as _},
};
use clap::ValueEnum;
use eyre::{Result, eyre};
use strum::IntoStaticStr;
use tokio::{
    select, signal,
    time::{Instant, sleep_until},
};

use crate::common::args::CommonArgs;

const PING_INTERVAL: Duration = Duration::from_secs(10);

#[derive(Clone, Copy, ValueEnum, IntoStaticStr)]
#[clap(rename_all = "UPPER")]
#[strum(serialize_all = "UPPERCASE")]
pub enum Verbosity {
    Keys,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
}

pub async fn log(
    common: CommonArgs,
    verbosity: Option<Verbosity>,
    file: Option<String>,
    line: Option<u64>,
    human_time: bool,
) -> Result<()> {
    let cjdns = cjdns::admin::connect(Some(common.with_auth())).await?;
    let response = cjdns_invoke_subscribe!(
        cjdns,
        "AdminLog_subscribe",
        level? = verbosity.map(Into::<&str>::into),
        file?,
        line? = line.map(|l| l as i64)
    )
    .await?;
    let mut stream = response.stream.ok_or_else(|| {
        log::info!("AdminLog_subscribe response: {:?}", response.message);
        eyre!("Admin log stream ID is missing")
    })?;
    let stream_id = response
        .message
        .get_str("streamId")
        .map_err(|e| eyre!("Failed to parse stream ID: {e}"))?;
    log::info!(r#"Subscribed to stream "{stream_id}""#);

    let mut next_ping = Instant::now() + PING_INTERVAL;
    loop {
        select! {
            res = signal::ctrl_c() => {
                match cjdns_invoke!(cjdns, "AdminLog_unsubscribe", streamId = stream_id).await {
                    Ok(_) => log::info!(r#"Unsubscribed from stream "{stream_id}" gracefully"#),
                    Err(err) => log::error!(r#"Failed to unsubscribe from stream "{stream_id}": {err}"#),
                }
                return res.map_err(|err| eyre!("Failed to listen to Ctrl+C: {err}"));
            }

            entry = stream.recv() => {
                if let Some(entry) = entry {
                    if let Err(error) = show_entry(entry, human_time) {
                        log::error!("Failed to display entry: {error}");
                    }
                } else {
                    log::warn!("CJDNS instance has closed the stream, exiting...");
                    return Ok(());
                }
            }

            _ = sleep_until(next_ping) => {
                if let Err(err) = cjdns_invoke!(cjdns, "ping").await {
                    log::error!("CJDNS instance ping error: {err}");
                }
                next_ping = Instant::now() + PING_INTERVAL;
            }
        }
    }
}

fn show_entry(entry: Dict<'_>, human_time: bool) -> Result<()> {
    let (time, level, file, line, message) = (
        entry.get_int("time").map_err(|e| eyre!(r#""time": {e}"#))?,
        entry
            .get_str("level")
            .map_err(|e| eyre!(r#""level": {e}"#))?,
        entry.get_str("file").map_err(|e| eyre!(r#""file": {e}"#))?,
        entry.get_int("line").map_err(|e| eyre!(r#""line": {e}"#))?,
        entry
            .get_str("message")
            .map_err(|e| eyre!(r#""message": {e}"#))?,
    );
    if human_time && let Some(time) = DateTime::from_timestamp_secs(time) {
        println!(
            "{} {level} {file}:{line} {message}",
            time.to_rfc3339_opts(SecondsFormat::Secs, true)
        );
    } else {
        println!("{time} {level} {file}:{line} {message}",);
    }
    Ok(())
}
