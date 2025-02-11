//! Crypto

#![allow(dead_code)] //TODO remove when done

pub mod cnoise;
pub mod crypto_auth;
pub mod crypto_header;
pub mod crypto_noise;
pub mod keys;
pub mod random;
pub mod replay_protector;
pub mod session;

mod utils {
    use cjdns::sodiumoxide::crypto::hash::sha256;
    use cjdns::sodiumoxide::crypto::scalarmult::curve25519;

    use super::keys::{PrivateKey, PublicKey};
    use super::wipe::Wipe;

    #[inline(always)]
    pub fn crypto_scalarmult_curve25519_base(pk: &PrivateKey) -> PublicKey {
        let scalar = pk.to_scalar();
        let curve25519::GroupElement(res) = curve25519::scalarmult_base(&scalar);
        scalar.wipe(); // Wipe sensitive data on the stack
        PublicKey::from(res)
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

    use cjdns::sodiumoxide::crypto::scalarmult::curve25519::Scalar;

    use crate::crypto::crypto_header::CryptoHeader;

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

    /// Perform a volatile write to the slice.
    #[inline(always)]
    fn volatile_set<T: Copy + Sized>(dst: &mut [T], src: T) {
        let count = dst.len();
        let dst = dst.as_mut_ptr();
        for i in 0..count {
            unsafe {
                let ptr = dst.add(i);
                ptr::write_volatile(ptr, src);
            }
        }
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

    impl Wipe for CryptoHeader {
        #[inline(always)]
        fn wipe(mut self) {
            // Prevent this write from being optimized away
            volatile_set(self.as_mut_bytes(), 0_u8);
            // Prevent reordering
            atomic_fence();
        }
    }
}

mod zero {
    /// Trait that defines zero-check operation.
    pub trait IsZero {
        /// Check whether this object is all-zeroes.
        fn is_zero(&self) -> bool;
    }

    impl IsZero for [u8; 32] {
        #[inline(always)]
        fn is_zero(&self) -> bool {
            *self == [0; 32]
        }
    }
}
