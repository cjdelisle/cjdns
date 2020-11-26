//! Error type from C part of the project.

#![allow(non_camel_case_types)]

/// Error structure.
///
/// Corresponds to C `struct Error_s` from `wire/Error.h`.
#[repr(C)]
pub struct Error_s {
    pub e: Error_e,
}

/// Error enum.
///
/// Corresponds to C `enum Error_e` from `wire/Error.h`.
#[repr(C)]
pub enum Error_e {
    /// No error, everything is ok.
    Error_NONE = 0,

    /// The switch label was malformed.
    Error_MALFORMED_ADDRESS = 1,

    /// Packet dropped because link is congested.
    Error_FLOOD = 2,

    /// Packet dropped because node has oversent its limit.
    Error_LINK_LIMIT_EXCEEDED = 3,

    /// Message too big to send.
    Error_OVERSIZE_MESSAGE = 4,

    /// Message smaller than expected headers.
    Error_RUNT = 5,

    /// Authentication failed.
    Error_AUTHENTICATION = 6,

    /// Header is invalid or checksum failed.
    Error_INVALID = 7,

    /// Message could not be sent to its destination through no fault of the sender.
    Error_UNDELIVERABLE = 8,

    /// The route enters and leaves through the same interface in one switch.
    Error_LOOP_ROUTE = 9,

    /// The switch is unable to represent the return path.
    Error_RETURN_PATH_INVALID = 10,

    /// Not invalid, but not something the code is able to handle.
    Error_UNHANDLED = 11,

    /// Too many messages, cannot handle.
    Error_OVERFLOW = 12,

    /// Something went wrong, it should not have happened.
    Error_INTERNAL = 13,
}
