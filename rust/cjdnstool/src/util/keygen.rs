use crate::common::{base32, utils};
use eyre::{bail, Result};
use sha2::{Digest, Sha512};
use cjdns::sodiumoxide::{crypto::box_, hex};

pub async fn keygen() -> Result<()> {
    if cjdns::sodiumoxide::init().is_err() {
        bail!("failed to initialize sodium library")
    }

    let (pubkey, privkey, hash) = loop {
        let pair = box_::gen_keypair();
        let hash = Sha512::digest(Sha512::digest(&pair.0));
        if hash[0] == 0xfc {
            break (pair.0, pair.1, hash);
        }
    };

    println!(
        "{} {}.k {}",
        hex::encode(privkey.0),
        base32::encode(&pubkey.0),
        utils::hash_to_ip6(hash)
    );
    Ok(())
}
