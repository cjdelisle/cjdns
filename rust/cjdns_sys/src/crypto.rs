//! Crypto

#![allow(dead_code)] //TODO remove when done

mod crypto_auth;
mod crypto_header;
mod keys;
mod random;
mod replay_protector;

mod utils {
    use sodiumoxide::crypto::hash::sha256;
    use sodiumoxide::crypto::scalarmult::curve25519;

    use super::keys::{PrivateKey, PublicKey};

    #[inline(always)]
    pub fn crypto_scalarmult_curve25519_base(pk: &PrivateKey) -> PublicKey {
        let PrivateKey(bytes) = pk;
        let scalar = curve25519::Scalar(bytes.clone());
        let curve25519::GroupElement(res) = curve25519::scalarmult_base(&scalar);
        PublicKey(res)
    }

    #[inline(always)]
    pub fn crypto_hash_sha256(data: &[u8]) -> [u8; 32] {
        let sha256::Digest(digest) = sha256::hash(data);
        digest
    }
}
