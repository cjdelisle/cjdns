//! Random numbers

pub use cjdns_crypto::random::DefaultRandom as SodiumRandom;
pub use cjdns_crypto::random::Random as Rand;

use crate::cffi::Random as CRandom;
use crate::cffi::Random_bytes;

pub enum Random {
    Sodium(SodiumRandom),
    Legacy(*mut CRandom),
    #[cfg(test)]
    Fake,
}

impl Random {
    #[inline]
    pub fn new_sodium() -> Result<Self, ()> {
        Ok(Random::Sodium(SodiumRandom::new()?))
    }

    #[inline]
    pub fn wrap_legacy(c_random: *mut CRandom) -> Self {
        Random::Legacy(c_random)
    }

    #[inline]
    pub fn random_bytes(&self, dest: &mut [u8]) {
        match self {
            Random::Sodium(r) => r.random_bytes(dest),
            Random::Legacy(r) => c_random_bytes(*r, dest),
            #[cfg(test)]
            Random::Fake => (0..dest.len()).for_each(|i| dest[i] = i as u8),
        }
    }
}

#[inline]
fn c_random_bytes(rand: *mut CRandom, dest: &mut [u8]) {
    unsafe {
        Random_bytes(rand, dest.as_mut_ptr(), dest.len() as u64);
    }
}

impl Rand for Random {
    #[inline]
    fn random_bytes(&self, dest: &mut [u8]) {
        self.random_bytes(dest);
    }
}