//! Identity debugging utils.
//!
//! Not yet properly implemented.

use std::ffi::c_void;

/// External opaque C Identity type. Should be used in a form of a pointer only.
pub struct Identity;

impl Identity {
    #[inline]
    pub fn set(_: *mut c_void) {
        // Not implemented
    }
}
