//! Message type.

use std::slice::{from_raw_parts, from_raw_parts_mut};

use thiserror::Error;

use crate::cffi;

/// A network message.
///
/// Wraps a pointer to C `struct Message` from `wire/Message.h`.
///
/// *Unsafe:* The original pointer *must* remain valid while this instance still in use.
#[derive(Clone)]
pub struct Message(*mut cffi::Message);

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

impl Message {
    /// Construct a Rust `Message` by wrapping a pointer to C `Message`.
    ///
    /// *Unsafe:* The original pointer *must* remain valid until this instance is dropped.
    #[inline]
    pub fn from_c_message(c_msg: *mut cffi::Message) -> Self {
        Message(c_msg)
    }

    /// Return original C `Message` pointer from this Rust `Message`.
    #[inline]
    pub fn as_c_message(&self) -> *mut cffi::Message {
        let &Message(c_msg) = self;
        c_msg
    }

    /// Get the message length.
    #[inline]
    pub fn len(&self) -> usize {
        let msg = unsafe { &mut (*self.0) };
        debug_assert!(msg.length >= 0);
        msg.length as usize
    }

    /// Get the message capacity.
    #[inline]
    pub fn cap(&self) -> usize {
        let msg = unsafe { &mut (*self.0) };
        debug_assert!(msg.capacity >= 0);
        msg.capacity as usize
    }

    /// Get the available padding size.
    #[inline]
    pub fn pad(&self) -> usize {
        let msg = unsafe { &mut (*self.0) };
        debug_assert!(msg.padding >= 0);
        msg.padding as usize
    }

    /// Get the read-only view into the message data as byte slice.
    #[inline]
    pub fn bytes(&self) -> &[u8] {
        let msg = unsafe { &mut (*self.0) };
        debug_assert!(!msg.bytes.is_null());
        debug_assert!(msg.length >= 0);
        let ptr = msg.bytes as *const u8;
        let len = msg.length as usize;
        unsafe { from_raw_parts(ptr, len) }
    }

    /// Get the mutable view into the message data as byte slice.
    #[inline]
    pub fn bytes_mut(&mut self) -> &mut [u8] {
        let msg = unsafe { &mut (*self.0) };
        debug_assert!(!msg.bytes.is_null());
        debug_assert!(msg.length >= 0);
        let ptr = msg.bytes;
        let len = msg.length as usize;
        unsafe { from_raw_parts_mut(ptr, len) }
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
        } else { // Fallback to slower byte slice copying
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
        } else { // Fallback to slower byte slice copying
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
        let msg = unsafe { &mut (*self.0) };
        let ptr = msg.bytes as usize;
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
        let msg = unsafe { &mut (*self.0) };
        let ptr = msg.bytes as usize;
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
        let msg = unsafe { &mut (*self.0) };

        debug_assert!(msg.length >= 0);
        debug_assert!(msg.padding >= 0);

        if amount > 0 {
            if msg.padding < amount {
                return Err(MessageError::BufferOverflow(amount, msg.length));
            }
        } else if amount < 0 {
            if msg.length < -amount {
                return Err(MessageError::BufferUnderflow(-amount, msg.length));
            }
        } else {
            return Ok(());
        }

        msg.length += amount;
        msg.capacity += amount;
        msg.bytes = (msg.bytes as isize - amount as isize) as *mut u8;
        msg.padding -= amount;

        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use crate::cffi;

    use super::Message;

    mod alloc {
        use crate::cffi::Allocator;

        pub(super) fn new_allocator(size_limit: u64) -> *mut Allocator {
            unsafe { crate::cffi::MallocAllocator__new(size_limit, "".as_ptr() as *const i8, 0) }
        }
    }

    #[test]
    fn test_message_bytes() {
        let alloc = alloc::new_allocator(1024);
        let c_msg = unsafe { cffi::Message_new(4, 5, alloc) };
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
        let alloc = alloc::new_allocator(1024);
        let c_msg = unsafe { cffi::Message_new(64, 64, alloc) };
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
        let alloc = alloc::new_allocator(1024);
        let c_msg = unsafe { cffi::Message_new(64, 64, alloc) };
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
