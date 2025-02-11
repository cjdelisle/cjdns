use crate::common::base32;
use eyre::{bail, Result};
use cjdns::sodiumoxide::{crypto::box_::SecretKey, hex};
use std::fmt::Write;

pub async fn priv2pub(privkeys: Vec<String>) -> Result<()> {
    const KEY_LEN: usize = 64;

    let mut output = String::new();
    for (index, privkey) in privkeys.into_iter().enumerate() {
        use std::cmp::Ordering::*;
        let adj = match privkey.chars().count().cmp(&KEY_LEN) {
            Less => Some("short"),
            Equal => None,
            Greater => Some("long"),
        };
        if let Some(adj) = adj {
            bail!(
                "argument {} [{}] is too {} to be a valid private key",
                index,
                privkey,
                adj
            );
        }
        if let Ok(raw) = hex::decode(&privkey) {
            let pubkey = SecretKey::from_slice(&raw)
                .unwrap() // already checked the length
                .public_key();
            writeln!(output, "{}.k", base32::encode(&pubkey.0))?;
        } else {
            bail!(
                "argument {} [{}] is not a valid hexadecimal string",
                index,
                privkey
            );
        }
    }

    print!("{output}");
    Ok(())
}
