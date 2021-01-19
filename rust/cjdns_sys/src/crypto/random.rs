//! Random numbers

use parking_lot::Once;

use sodiumoxide::{self, randombytes};

pub struct Random;

impl Random {
    pub fn new() -> Result<Self, ()> {
        if Self::init_sodiumoxide() {
            Ok(Random {})
        } else {
            Err(())
        }
    }

    fn init_sodiumoxide() -> bool {
        static INIT_SODIUMOXIDE: Once = Once::new();
        static mut INITIALIZED: bool = false;

        INIT_SODIUMOXIDE.call_once(|| {
            let success = sodiumoxide::init().is_ok();
            unsafe {
                // Safe because of `call_once()` guarantees
                INITIALIZED = success;
            }
        });

        // Safe because of `call_once()` guarantees
        unsafe { INITIALIZED }
    }

    #[inline(always)]
    pub fn random_bytes(&self, dest: &mut [u8]) {
        randombytes::randombytes_into(dest);
    }
}
