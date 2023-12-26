use crate::common::utils;
use anyhow::{anyhow, bail, Result};
use std::fmt::Write;

pub async fn key2ip6(pubkeys: Vec<String>) -> Result<()> {
    const KEY_LEN: usize = 54;

    let mut output = String::new();
    for (index, pubkey) in pubkeys.into_iter().enumerate() {
        use std::cmp::Ordering::*;
        let adj = match pubkey.chars().count().cmp(&KEY_LEN) {
            Less => Some("short"),
            Equal => None,
            Greater => Some("long"),
        };
        if let Some(adj) = adj {
            bail!(
                "argument {} [{}] is too {} to be a valid public key",
                index,
                pubkey,
                adj
            );
        }
        let ip6 = utils::key_to_ip6(&pubkey, false)
            .map_err(|e| anyhow!("argument {} [{}]: {}", index, pubkey, e))?;
        writeln!(output, "{pubkey} {ip6}")?;
    }

    print!("{output}");
    Ok(())
}
