//! `ByteString` is a String-like type that can hold arbitrary byte sequence
//! which is considered a text string in an unknown encoding.
//!
//! Required to safely exchange text data with C code, because that text data
//! is not always a valid UTF-8 and can not be stored in a `String`.

use std::fmt;
use std::ops::Deref;

/// String-like type that can hold arbitrary byte sequence which is considered
/// a text string in an unknown encoding.
///
/// Required to safely exchange text data with C code, because that text data
/// is not always a valid UTF-8 and can not be stored in a `String`.
///
/// It is somehow similar to `OsString` in its purpose, but always uses
/// byte sequence as internal representation, which is not always true
/// for the `OsString` - its internal representation is platform-dependent.
///
/// Wherever possible, use standard `String` instead, and only use this type
/// as a last resort.
#[derive(Default, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct ByteString(pub Vec<u8>);

impl ByteString {
    #[inline]
    pub fn empty() -> Self {
        ByteString(Vec::new())
    }

    /// Consume this byte string and convert it into human-readable `String`.
    ///
    /// Use this only for debug-relate things such as logging.
    ///
    /// Don't use this for comparisons because anything with a non-utf8 char will be equal
    /// to the same hex value, just for debugging...
    pub fn into_debug_string(self) -> String {
        let ByteString(bytes) = self;
        match String::from_utf8(bytes) {
            Ok(s) => s,
            Err(e) => hex::encode(e.into_bytes()),
        }
    }
}

impl From<String> for ByteString {
    #[inline]
    fn from(s: String) -> Self {
        ByteString(s.into_bytes())
    }
}

impl Deref for ByteString {
    type Target = [u8];

    #[inline]
    fn deref(&self) -> &[u8] {
        let ByteString(bytes) = self;
        bytes.as_slice()
    }
}

impl fmt::Debug for ByteString {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let s = self.clone().into_debug_string();
        f.write_str(&s)
    }
}
