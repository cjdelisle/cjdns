//! Message type from C part of the project.

#![allow(non_snake_case)]

use std::os::raw::c_int;

use crate::external::memory::allocator::*;

/// A network message.
///
/// Corresponds to C `struct Message` from `wire/Message.h`.
#[repr(C)]
pub struct Message {
    /// The length of the message.
    pub length: i32,

    /// The number of bytes of padding BEFORE where bytes begins.
    pub padding: i32,

    /// The content.
    pub bytes: *const u8,

    /// Amount of bytes of storage space available in the message.
    pub capacity: i32,

    /// When sending/receiving a Message on a unix socket, a file descriptor to attach.
    /// Caveat: In order to maintain backward compatibility with a Message which is
    /// allocated using calloc, file descriptor 0 is referred to by -1
    pub associatedFd: c_int,

    // The problem here is that this `cfg(PARANOIA)`
    // does not automatically match C's preprocessor `#define PARANOIA`
    // so had to be manually taken care of.
    // In case of mismatch the C's and Rust's structures would not match either,
    // and the universe will collapse.
    /// This is used inside of Iface.h to support Iface_next()
    #[cfg(PARANOIA)]
    pub currentIface: *const crate::external::interface::iface::Iface,

    /// The allocator which allocated space for this message.
    pub alloc: *mut Allocator,
}
