//! Network interface from C part of the project.

#![allow(non_camel_case_types, non_snake_case)]

use crate::external::wire::error::*;
use crate::external::wire::message::*;

/// A network interface representation structure.
///
/// Corresponds to C `struct Iface` from `interface/Iface.h`.
#[repr(C)]
pub struct Iface {
    /// Send a message through this interface.
    pub send: Iface_Callback,

    // The problem here is that this `cfg(PARANOIA)`
    // does not automatically match C's preprocessor `#define PARANOIA`
    // so had to be manually taken care of.
    // In case of mismatch the C's and Rust's structures would not match either,
    // and the universe will collapse.
    /// This is for checking currentMsg Iface_next() has not been called incorrectly.
    #[cfg(PARANOIA)]
    currentMsg: *const Message,

    /// Interface to which this one is connected (if connected).
    connectedIf: *mut Iface,
}

pub type Iface_Callback =
    unsafe extern "C" fn(message: *const Message, thisInterface: *mut Iface) -> Error_s;
