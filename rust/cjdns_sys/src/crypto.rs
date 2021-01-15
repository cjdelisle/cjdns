//! Crypto

#![allow(dead_code)] //TODO remove when done

pub mod crypto_auth;
pub mod crypto_header;
pub mod keys;
pub mod random;
pub mod replay_protector;

mod utils {
    use sodiumoxide::crypto::hash::sha256;
    use sodiumoxide::crypto::scalarmult::curve25519;

    use super::keys::{PrivateKey, PublicKey};
    use super::wipe::Wipe;

    #[inline(always)]
    pub fn crypto_scalarmult_curve25519_base(pk: &PrivateKey) -> PublicKey {
        let PrivateKey(bytes) = pk;
        let scalar = curve25519::Scalar(bytes.clone());
        let curve25519::GroupElement(res) = curve25519::scalarmult_base(&scalar);
        scalar.wipe(); // Wipe sensitive data on the stack
        PublicKey(res)
    }

    #[inline(always)]
    pub fn crypto_hash_sha256(data: &[u8]) -> [u8; 32] {
        let sha256::Digest(digest) = sha256::hash(data);
        digest
    }
}

mod wipe {
    use std::ptr;
    use std::sync::atomic;

    use sodiumoxide::crypto::scalarmult::curve25519::Scalar;

    /// Trait that defines `wipe` operation which securely overwrites sensitive
    /// data such as cryptographic keys with zeroes.
    pub trait Wipe {
        /// Consume this object and overwrite all its memory with zeroes.
        /// Can help avoiding leaving traces of sensitive data on the stack
        /// or heap.
        fn wipe(self);
    }

    /// Perform a volatile write to the destination.
    #[inline(always)]
    fn volatile_write<T: Copy + Sized>(dst: &mut T, src: T) {
        unsafe { ptr::write_volatile(dst, src) }
    }

    /// Use fences to prevent accesses from being reordered before this
    /// point, which should hopefully help ensure that all accessors
    /// see zeroes after this point.
    #[inline(always)]
    fn atomic_fence() {
        atomic::compiler_fence(atomic::Ordering::SeqCst);
    }

    impl Wipe for Scalar {
        #[inline(always)]
        fn wipe(mut self) {
            // Prevent this write from being optimized away
            volatile_write(&mut self.0, [0; 32]);
            // Prevent reordering
            atomic_fence();
        }
    }
}
