//! Message type.

use thiserror::Error;

/// A network message.
///
/// Can be converted to/from to C `struct Message` from `wire/Message.h`.
#[derive(Clone)]
pub struct Message {
    /// Actual data starts at offset `padding`.
    data: Box<[u8]>,

    /// The number of bytes of padding BEFORE the actual message data.
    /// Useful to prepend additional data at no cost.
    padding: usize,
}

#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum MessageError {
    #[error("Can't prepend {0} bytes, only {1} bytes capacity available")]
    InsufficientPadding(usize, usize),

    #[error("Can't take {0} bytes from the message, only {1} bytes available")]
    InsufficientLength(usize, usize),
}

pub type Result<T> = std::result::Result<T, MessageError>;

impl Message {
    /// Create new message with zero-initialized content of a specified size.
    /// The `prepend_capacity` bytes is added to the message capacity *before* the actual data,
    /// so that the message can be easily prepended with more data.
    #[inline]
    pub fn new(size: usize, prepend_capacity: usize) -> Self {
        Message {
            data: vec![0_u8; size + prepend_capacity].into_boxed_slice(),
            padding: prepend_capacity,
        }
    }

    /// Get the message length.
    #[inline]
    pub fn len(&self) -> usize {
        debug_assert!(self.data.len() >= self.padding);
        self.data.len() - self.padding
    }

    /// Get the available padding size.
    #[inline]
    pub fn pad(&self) -> usize {
        self.padding
    }

    /// Get the read-only view into the message as byte slice.
    #[inline]
    pub fn bytes(&self) -> &[u8] {
        debug_assert!(self.padding <= self.data.len());
        &self.data[self.padding..]
    }

    /// Get the mutable view into the message as byte slice.
    #[inline]
    pub fn bytes_mut(&mut self) -> &mut [u8] {
        debug_assert!(self.padding <= self.data.len());
        &mut self.data[self.padding..]
    }

    /// Push additional data `bytes` *before* the message's existing data.
    /// The available padding must be enough to accommodate additional data,
    /// otherwise error is returned.
    pub fn push(&mut self, bytes: &[u8]) -> Result<()> {
        if bytes.len() > self.padding {
            return Err(MessageError::InsufficientPadding(bytes.len(), self.padding));
        }
        self.padding -= bytes.len();
        debug_assert!(self.padding <= self.data.len());
        let dest = &mut self.data[self.padding..self.padding + bytes.len()];
        dest.copy_from_slice(bytes);
        Ok(())
    }

    /// Pop specified number of bytes from the beginning of the message.
    /// The message must be big enough, otherwise error is returned.
    pub fn pop(&mut self, count: usize) -> Result<Vec<u8>> {
        if self.len() < count {
            return Err(MessageError::InsufficientLength(count, self.len()));
        }
        debug_assert!(self.padding <= self.data.len());
        let src = &self.data[self.padding..self.padding + count];
        let res = Vec::from(src);
        self.padding += count;
        Ok(res)
    }

    /// Peek the specified number of bytes from the beginning of the message.
    /// Error is returned is the message is too short.
    pub fn peek(&self, count: usize) -> Result<&[u8]> {
        if self.len() < count {
            return Err(MessageError::InsufficientLength(count, self.len()));
        }
        debug_assert!(self.padding <= self.data.len());
        let res = &self.data[self.padding..self.padding + count];
        Ok(res)
    }

    /// Peek the specified number of bytes from the beginning of the message as a mutable slice.
    /// Error is returned is the message is too short.
    pub fn peek_mut(&mut self, count: usize) -> Result<&mut [u8]> {
        if self.len() < count {
            return Err(MessageError::InsufficientLength(count, self.len()));
        }
        debug_assert!(self.padding <= self.data.len());
        let res = &mut self.data[self.padding..self.padding + count];
        Ok(res)
    }
}

#[cfg(test)]
mod tests {
    use super::Message;

    #[test]
    fn test_message() {
        let mut msg = Message::new(4, 5);
        assert_eq!(msg.len(), 4);
        assert_eq!(msg.pad(), 5);
        assert_eq!(msg.bytes().len(), 4);
        assert_eq!(msg.bytes_mut().len(), 4);
        msg.bytes_mut().copy_from_slice(&[1, 2, 3, 4]);
        assert_eq!(msg.bytes(), &[1, 2, 3, 4]);
        assert!(msg.push(&[42, 43]).is_ok());
        assert_eq!(msg.bytes(), &[42, 43, 1, 2, 3, 4]);
        assert_eq!(msg.len(), 6);
        assert_eq!(msg.pad(), 3);
        assert!(msg.push(&[10, 11, 12, 13]).is_err());
        assert_eq!(msg.peek(3), Ok(&[42_u8, 43, 1][..]));
        assert_eq!(msg.peek_mut(3), Ok(&mut [42_u8, 43, 1][..]));
        assert!(msg.peek_mut(10).is_err());
        assert_eq!(msg.pop(4), Ok(vec![42, 43, 1, 2]));
        assert_eq!(msg.bytes(), &[3, 4]);
        assert_eq!(msg.pop(2), Ok(vec![3, 4]));
        assert_eq!(msg.len(), 0);
        assert_eq!(msg.pad(), 9);
    }
}
