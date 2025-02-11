//! Random numbers

pub use cjdns::crypto::random::DefaultRandom as SodiumRandom;
pub use cjdns::crypto::random::Random as Rand;

use crate::cffi::Random_t;
use crate::cffi::Random_bytes_fromRust;
use crate::gcl::Protected;

pub enum Random {
    Sodium(SodiumRandom),
    Legacy(Protected<*mut Random_t>),
    #[cfg(test)]
    Fake,
}

impl Random {
    #[inline]
    pub fn new_sodium() -> Result<Self, ()> {
        Ok(Random::Sodium(SodiumRandom::new()?))
    }

    #[inline]
    pub fn wrap_legacy(c_random: *mut Random_t) -> Self {
        Random::Legacy(Protected::new(c_random))
    }

    #[inline]
    pub fn random_bytes(&self, dest: &mut [u8]) {
        match self {
            Random::Sodium(r) => r.random_bytes(dest),
            Random::Legacy(r) => {
                let r_l = r.lock();
                c_random_bytes(*r_l, dest)
            }
            #[cfg(test)]
            Random::Fake => (0..dest.len()).for_each(|i| dest[i] = i as u8),
        }
    }
}

#[inline]
fn c_random_bytes(rand: *mut Random_t, dest: &mut [u8]) {
    unsafe {
        Random_bytes_fromRust(rand, dest.as_mut_ptr(), dest.len() as u64);
    }
}

impl Rand for Random {
    #[inline]
    fn random_bytes(&self, dest: &mut [u8]) {
        self.random_bytes(dest);
    }
}