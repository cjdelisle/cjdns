//! Conversion between C and Rust `Message`.

use std::ptr::{null_mut, slice_from_raw_parts};

use crate::cffi;
use crate::external::memory::allocator;
use crate::interface::wire::message::Message;

impl Message {
    /// Construct a Rust `Message` from C `Message`.
    /// Data is cloned as well, so that the original message
    /// is no longer needed.
    pub fn from_c_message(msg: *mut cffi::Message) -> Self {
        let msg = unsafe { &*msg };
        let len = msg.length as usize;
        let pad = msg.padding as usize;
        let data = unsafe { &*slice_from_raw_parts(msg.bytes, len) };
        let mut res = Message::new(len, pad);
        res.bytes_mut().copy_from_slice(data);
        res
    }

    /// Return C `Message` view into the data of this Rust `Message`.
    /// The returned reference has the same lifetime as `self` to enforce
    /// keeping this message alive while the view is used.
    /// Data can be modified through this view, though it's size must not be changed.
    pub fn as_c_message(&mut self, alloc: *mut cffi::Allocator) -> &mut cffi::Message {
        // We are borrowing data from `self`,
        // so that we need to return a reference with the same lifetime as `self`.
        // Another option would be to clone data, and return plain `cffi::Message`.
        let data = self.bytes_mut();
        let data = &mut data[0] as *mut u8;

        allocator::adopt(
            alloc,
            cffi::Message {
                length: self.len() as i32,
                padding: 0, // We are passing only message itself, not the padding
                bytes: data,
                capacity: self.len() as i32,
                associatedFd: 0,
                currentIface: null_mut(),
                alloc
            },
        )
    }
}
