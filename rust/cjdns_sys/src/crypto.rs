//! Crypto

#![allow(dead_code)] //TODO remove when done

mod crypto_auth;
mod crypto_header;
mod keys;
mod random;
mod replay_protector;


mod utils {
    use super::keys::{PrivateKey, PublicKey};

    pub fn crypto_scalarmult_curve25519_base(_: &PrivateKey) -> PublicKey {
        todo!()
    }

    pub fn crypto_hash_sha256(_data: &[u8]) -> [u8; 32] {
        todo!()
    }
}
