//! Message type.

use std::slice::{from_raw_parts, from_raw_parts_mut};
use std::i32;

use thiserror::Error;

use crate::cffi;
use crate::rffi::allocator::{self, Allocator};

/// A network message.
///
/// Wraps a pointer to C `struct Message` from `wire/Message.h`.
///
/// *Unsafe:* The original pointer *must* remain valid while this instance still in use.
pub struct Message {
    msg: *mut cffi::Message,
    alloc: Option<Allocator>,
}
// Needed to use Message in an any async function
unsafe impl Sync for Message {}
unsafe impl Send for Message {}

#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum MessageError {
    #[error("Can't prepend {0} bytes, only {1} bytes capacity available")]
    InsufficientPadding(usize, usize),

    #[error("Can't take {0} bytes from the message, only {1} bytes available")]
    InsufficientLength(usize, usize),

    #[error("Buffer overflow adding {0} to length {1}")]
    BufferOverflow(i32, i32),

    #[error("Buffer underflow, amount={0}, length={1}")]
    BufferUnderflow(i32, i32),

    #[error("Buffer misaligned: item size {0}, required alignment {1}")]
    InvalidAlign(usize, usize),
}

pub type Result<T> = std::result::Result<T, MessageError>;

impl std::fmt::Debug for Message {
     fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
         f.debug_struct("Message")
         .field("ptr", &format!("{:p}", self.msg))
         .field("length", &self.len())
         .field("cap", &self.cap())
         .field("padding", &self.pad())
         .finish()
     }
}

impl Message {
    /// Create a new message with a self-contained allocator,
    /// this message is thus "owned" but Rust.
    pub fn new(padding: usize) -> Self {
        let alloc = allocator::new!();
        let mut child = allocator::child!(alloc);
        let mut out = Self::anew(padding, &mut child);
        out.alloc = Some(alloc);
        out
    }

    pub fn anew(padding: usize, alloc: &mut Allocator) -> Self {
        unsafe { Message { msg: cffi::Message_new(0, padding as u32, alloc.c()), alloc: None } }
    }

    /// Create empty new message with the given amount of free space,
    /// using the same allocator as `self` does.
    ///
    /// Note: this function does *NOT* clone original message,
    /// the resulting message will be empty.
    pub fn new_same_alloc(&self, padding: usize) -> Self {
        unsafe { Message { msg: cffi::Message_new(0, padding as u32, (*self.msg)._alloc), alloc: None } }
    }

    /// Construct a Rust `Message` by wrapping a pointer to C `Message`.
    ///
    /// *Unsafe:* The original pointer *must* remain valid until this instance is dropped.
    #[inline]
    pub fn from_c_message(c_msg: *mut cffi::Message) -> Self {
        let my_alloc = allocator::new!();
        my_alloc.adopt_alloc(allocator::rs(unsafe { (*c_msg)._alloc }));
        Message { msg: c_msg, alloc: Some(my_alloc) }
    }

    /// Return original C `Message` pointer from this Rust `Message`.
    #[inline]
    pub fn as_c_message(&self) -> *mut cffi::Message {
        let &Message{ msg, .. } = self;
        msg
    }

    /// Get the message length.
    #[inline]
    pub fn len(&self) -> usize {
        let msg = unsafe { &mut (*self.msg) };
        debug_assert!(msg._length >= 0);
        msg._length as usize
    }

    /// Get the message capacity.
    #[inline]
    pub fn cap(&self) -> usize {
        let msg = unsafe { &mut (*self.msg) };
        debug_assert!(msg._capacity >= 0);
        msg._capacity as usize
    }

    /// Get the available padding size.
    #[inline]
    pub fn pad(&self) -> usize {
        let msg = unsafe { &mut (*self.msg) };
        debug_assert!(msg._padding >= 0);
        msg._padding as usize
    }

    /// Check the message data alignment.
    #[inline]
    pub fn is_aligned_to(&self, align: usize) -> bool {
        self.data_ptr() % align == 0
    }

    /// Check the message data alignment.
    #[inline]
    pub fn data_ptr(&self) -> usize {
        let msg = unsafe { &mut (*self.msg) };
        msg.msgbytes as usize
    }

    /// Get the read-only view into the message data as byte slice.
    #[inline]
    pub fn bytes(&self) -> &[u8] {
        let msg = unsafe { &mut (*self.msg) };
        debug_assert!(!msg.msgbytes.is_null());
        debug_assert!(msg._length >= 0);
        let ptr = msg.msgbytes as *const u8;
        let len = msg._length as usize;
        unsafe { from_raw_parts(ptr, len) }
    }

    /// Get the mutable view into the message data as byte slice.
    #[inline]
    pub fn bytes_mut(&mut self) -> &mut [u8] {
        let msg = unsafe { &mut (*self.msg) };
        debug_assert!(!msg.msgbytes.is_null());
        debug_assert!(msg._length >= 0);
        let ptr = msg.msgbytes;
        let len = msg._length as usize;
        unsafe { from_raw_parts_mut(ptr, len) }
    }

    /// Allocate `count` additional data bytes *before* the message's existing data.
    /// These bytes are not initialized and will contain whatever the Message last held.
    /// The available padding must be enough to accommodate additional bytes
    /// otherwise an error is returned.
    pub fn allocate_uninitialized(&mut self, count: usize) -> Result<&mut [u8]> {
        if count > self.pad() {
            return Err(MessageError::InsufficientPadding(count, self.pad()));
        }
        self.shift(count as i32)?;
        let data = self.bytes_mut();
        let dest = &mut data[0..count];
        Ok(dest)
    }

    /// Change the length of the message. This can only be done within the message's
    /// capacity or else an error will occur.
    pub fn set_len(&mut self, new_size: usize) -> Result<()> {
        if new_size > self.cap() {
            return Err(MessageError::InsufficientLength(new_size, self.cap()));
        }
        unsafe {
            let mut msg = &mut (*self.msg);
            msg._length = new_size as i32;
        }
        Ok(())
    }

    /// Push additional data `bytes` *before* the message's existing data.
    /// The available padding must be enough to accommodate additional data,
    /// otherwise error is returned.
    pub fn push_bytes(&mut self, bytes: &[u8]) -> Result<()> {
        let count = bytes.len();
        debug_assert!(count < i32::MAX as usize);
        if count > self.pad() {
            return Err(MessageError::InsufficientPadding(count, self.pad()));
        }
        self.shift(count as i32)?;
        let data = self.bytes_mut();
        let dest = &mut data[0..count];
        dest.copy_from_slice(bytes);
        Ok(())
    }

    /// Pop specified number of bytes from the beginning of the message.
    /// The message must be big enough, otherwise error is returned.
    pub fn pop_bytes(&mut self, count: usize) -> Result<Vec<u8>> {
        debug_assert!(count < i32::MAX as usize);
        if self.len() < count {
            return Err(MessageError::InsufficientLength(count, self.len()));
        }
        let data = self.bytes();
        let src = &data[0..count];
        let res = Vec::from(src);
        self.shift(-(count as i32))?;
        Ok(res)
    }

    /// Discard specified number of bytes from the beginning of the message.
    /// The message must be big enough, otherwise error is returned.
    pub fn discard_bytes(&mut self, count: usize) -> Result<()> {
        debug_assert!(count < i32::MAX as usize);
        if self.len() < count {
            return Err(MessageError::InsufficientLength(count, self.len()));
        }
        self.shift(-(count as i32))?;
        Ok(())
    }

    /// Peek the specified number of bytes from the beginning of the message as a byte slice.
    /// Error is returned if the message is too short.
    pub fn peek_bytes(&self, count: usize) -> Result<&[u8]> {
        if self.len() < count {
            return Err(MessageError::InsufficientLength(count, self.len()));
        }
        let data = self.bytes();
        Ok(&data[0..count])
    }

    /// Peek the specified number of bytes from the beginning of the message as a mutable byte slice.
    /// Error is returned if the message is too short.
    pub fn peek_bytes_mut(&mut self, count: usize) -> Result<&mut [u8]> {
        if self.len() < count {
            return Err(MessageError::InsufficientLength(count, self.len()));
        }
        let data = self.bytes_mut();
        Ok(&mut data[0..count])
    }

    /// Push data item of type `T` *before* the message's existing data.
    /// The available padding must be enough to accommodate additional data,
    /// otherwise error is returned.
    pub fn push<T>(&mut self, mut value: T) -> Result<()> {
        let size = std::mem::size_of_val(&value);
        debug_assert!(size < i32::MAX as usize);
        if size > self.pad() {
            return Err(MessageError::InsufficientPadding(size, self.pad()));
        }

        self.shift(size as i32)?;

        // Try to use direct copying (faster) if the buffer is properly aligned
        if let Some(dest) = self.data_ref_mut::<T>() {
            // Swap uninitialized buffer space with the value
            std::mem::swap(dest, &mut value);
            // Discard without dropping whatever value we got from uninitialized buffer
            std::mem::forget(value);
        } else {
            // Fallback to slower byte slice copying
            let bytes = self.bytes_mut();
            let dest = &mut bytes[0..size];
            let value_ptr = &value as *const T as *const u8;
            let src = unsafe { from_raw_parts(value_ptr, size) };
            dest.copy_from_slice(src);
        }

        Ok(())
    }

    /// Pop data item of type `T` from the beginning of the message.
    /// The message must be big enough, otherwise error is returned.
    pub fn pop<T: Default>(&mut self) -> Result<T> {
        let size = std::mem::size_of::<T>();
        debug_assert!(size < i32::MAX as usize);
        if size > self.len() {
            return Err(MessageError::InsufficientLength(size, self.len()));
        }

        let mut res;

        // Try to use direct copying (faster) if the buffer is properly aligned
        if let Some(src) = self.data_ref_mut::<T>() {
            // Take the value leaving `Default::default()` on its place
            res = std::mem::take(src);
        } else {
            // Fallback to slower byte slice copying
            let bytes = self.bytes();
            let src = &bytes[0..size];
            res = T::default();
            let res_ptr = &mut res as *mut T as *mut u8;
            let dest = unsafe { from_raw_parts_mut(res_ptr, size) };
            dest.copy_from_slice(src);
        }

        self.shift(-(size as i32))?;

        Ok(res)
    }

    /// Discard data item of type `T` from the beginning of the message.
    /// The message must be big enough, otherwise error is returned.
    pub fn discard<T: Default>(&mut self) -> Result<()> {
        let size = std::mem::size_of::<T>();
        debug_assert!(size < i32::MAX as usize);
        if size > self.len() {
            return Err(MessageError::InsufficientLength(size, self.len()));
        }

        self.shift(-(size as i32))?;

        Ok(())
    }

    /// Peek the item of type `T` at the beginning of the message as a reference.
    /// Error is returned if the message is too short or buffer pointer is misaligned.
    pub fn peek<T>(&self) -> Result<&T> {
        let size = std::mem::size_of::<T>();
        if size > self.len() {
            return Err(MessageError::InsufficientLength(size, self.len()));
        }
        if let Some(data_ref) = self.data_ref::<T>() {
            Ok(data_ref)
        } else {
            return Err(MessageError::InvalidAlign(size, std::mem::align_of::<T>()));
        }
    }

    /// Peek the item of type `T` at the beginning of the message as a mutable reference.
    /// Error is returned if the message is too short or buffer pointer is misaligned.
    pub fn peek_mut<T>(&mut self) -> Result<&mut T> {
        let size = std::mem::size_of::<T>();
        if size > self.len() {
            return Err(MessageError::InsufficientLength(size, self.len()));
        }
        if let Some(data_ref) = self.data_ref_mut::<T>() {
            Ok(data_ref)
        } else {
            return Err(MessageError::InvalidAlign(size, std::mem::align_of::<T>()));
        }
    }

    #[inline]
    fn data_ref<T>(&self) -> Option<&T> {
        let msg = unsafe { &mut (*self.msg) };
        let ptr = msg.msgbytes as usize;
        let align = std::mem::align_of::<T>();
        if ptr % align == 0 {
            let ptr = ptr as *const T;
            let data = unsafe { &*ptr };
            Some(data)
        } else {
            None
        }
    }

    #[inline]
    fn data_ref_mut<T>(&mut self) -> Option<&mut T> {
        let msg = unsafe { &mut (*self.msg) };
        let ptr = msg.msgbytes as usize;
        let align = std::mem::align_of::<T>();
        if ptr % align == 0 {
            let ptr = ptr as *mut T;
            let data = unsafe { &mut *ptr };
            Some(data)
        } else {
            None
        }
    }

    fn shift(&mut self, amount: i32) -> Result<()> {
        let msg = unsafe { &mut (*self.msg) };

        debug_assert!(msg._length >= 0);
        debug_assert!(msg._padding >= 0);

        if amount > 0 {
            if msg._padding < amount {
                return Err(MessageError::BufferOverflow(amount, msg._length));
            }
        } else if amount < 0 {
            if msg._length < -amount {
                return Err(MessageError::BufferUnderflow(-amount, msg._length));
            }
        } else {
            return Ok(());
        }

        msg._length += amount;
        msg._capacity += amount;
        msg.msgbytes = (msg.msgbytes as isize - amount as isize) as *mut u8;
        msg._padding -= amount;

        Ok(())
    }

    /// Clear the message: discard all data and set size to 0.
    pub fn clear(&mut self) {
        unsafe {
            let mut msg = &mut (*self.msg);
            msg._length = msg._capacity;
        }
        let size = self.len() as i32;
        if size > 0 {
            self.shift(-size).expect("clear");
        }
    }

    /// Set the accompanying file descriptor in this message
    pub fn set_fd(&mut self, fd: i32) {
        unsafe {
            (*self.msg)._associatedFd = match fd {
                0 => -1,
                -1 => 0,
                fd => fd
            };
        }
    }

    /// Get the accompanying file descriptor from this message
    pub fn get_fd(&self) -> i32 {
        unsafe {
            match (*self.msg)._associatedFd {
                0 => -1,
                -1 => 0,
                fd => fd
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::cffi;

    use super::Message;
    use crate::rffi::allocator;

    #[test]
    fn test_message_bytes() {
        let mut alloc = allocator::new!();
        let c_msg = unsafe { cffi::Message_new(4, 5, alloc.c()) };
        let mut msg = Message::from_c_message(c_msg);
        assert_eq!(msg.len(), 4);
        assert_eq!(msg.pad(), 5);
        assert_eq!(msg.bytes().len(), 4);
        assert_eq!(msg.bytes_mut().len(), 4);
        msg.bytes_mut().copy_from_slice(&[1, 2, 3, 4]);
        assert_eq!(msg.bytes(), &[1, 2, 3, 4]);
        assert!(msg.push_bytes(&[42, 43]).is_ok());
        assert_eq!(msg.bytes(), &[42, 43, 1, 2, 3, 4]);
        assert_eq!(msg.len(), 6);
        assert_eq!(msg.pad(), 3);
        assert!(msg.push_bytes(&[10, 11, 12, 13]).is_err());
        assert_eq!(msg.peek_bytes(3), Ok(&[42_u8, 43, 1][..]));
        assert_eq!(msg.peek_bytes_mut(3), Ok(&mut [42_u8, 43, 1][..]));
        assert!(msg.peek_bytes_mut(10).is_err());
        assert_eq!(msg.pop_bytes(4), Ok(vec![42, 43, 1, 2]));
        assert_eq!(msg.bytes(), &[3, 4]);
        assert_eq!(msg.pop_bytes(2), Ok(vec![3, 4]));
        assert_eq!(msg.len(), 0);
        assert_eq!(msg.pad(), 9);
    }

    #[test]
    fn test_message_push_pop() {
        let mut alloc = allocator::new!();
        let c_msg = unsafe { cffi::Message_new(64, 64, alloc.c()) };
        let mut msg = Message::from_c_message(c_msg);
        assert_eq!(msg.len(), 64);
        assert_eq!(msg.push(0x12345678_u32), Ok(()));
        assert_eq!(msg.len(), 68);
        assert_eq!(msg.pop(), Ok(0x5678_u16));
        assert_eq!(msg.len(), 66);
        assert_eq!(msg.peek(), Ok(&0x1234_u16));
        assert_eq!(msg.pop(), Ok(0x1234_u16));
        assert_eq!(msg.len(), 64);
    }

    #[test]
    fn test_message_push_pop_unaligned() {
        let mut alloc = allocator::new!();
        let c_msg = unsafe { cffi::Message_new(64, 64, alloc.c()) };
        let mut msg = Message::from_c_message(c_msg);
        assert_eq!(msg.len(), 64);

        // Break the alignment
        assert_eq!(msg.push(0xFF_u8), Ok(()));

        // Push 4 bytes unaligned
        assert_eq!(msg.push(0x12345678_u32), Ok(()));

        // Peek can't read unaligned data - this is expected
        assert!(msg.peek::<u32>().is_err());
        assert!(msg.peek_mut::<u32>().is_err());

        // Break more
        assert_eq!(msg.push(0xEE_u8), Ok(()));

        // Pop 4 bytes unaligned
        assert_eq!(msg.pop(), Ok(0x345678EE_u32));
    }
}
