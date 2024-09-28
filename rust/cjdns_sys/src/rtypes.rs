#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

use std::sync::Arc;

use libc::c_char;
use num_enum::{IntoPrimitive, TryFromPrimitive};

use crate::{cffi::{self, Iface_t, String_t}, rffi::event_loop::EventLoop, util::identity::Identity};

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct RTypes_IfWrapper_t {
    pub internal: *mut Iface_t,
    pub external: *mut Iface_t,
}

#[repr(C)]
pub struct RTypes_StrList_t {
    pub len: usize,
    pub items: *mut *mut String_t,
}

#[repr(C)]
pub struct RTypes_Seeder_DnsSeed_t {
    pub seed: *mut c_char,
    pub snode_trusted: bool,
}

#[repr(C)]
pub struct RTypes_Seeder_DnsSeeds_t {
    pub len: usize,
    pub items: *mut RTypes_Seeder_DnsSeed_t,
}

#[repr(C)]
#[derive(Copy, Clone, PartialEq, Eq, Debug)]
pub enum RTypes_CryptoAuth_State_t {
    /// New CryptoAuth session, has not sent or received anything
    Init = 0,

    /// Sent a hello message, waiting for reply
    SentHello = 1,

    /// Received a hello message, have not yet sent a reply
    ReceivedHello = 2,

    /// Received a hello message, sent a key message, waiting for the session to complete
    SentKey = 3,

    /// Sent a hello message, received a key message, may or may not have sent some data traffic
    /// but no data traffic has yet been received
    ReceivedKey = 4,

    /// Received data traffic, session is in run state
    Established = 100,
}

#[repr(C)]
#[derive(Copy, Clone, PartialEq, Eq, Debug)]
pub struct RTypes_CryptoStats_t {
    /// Number of packets which were lost
    pub lost_packets: u64,

    /// Number of packets which were received but could not be validated
    pub received_unexpected: u64,

    /// Number of packets which were received (since last session setup)
    pub received_packets: u64,

    /// Number of packets which were received that were duplicates
    pub duplicate_packets: u64,

    /// True if the session is using the Noise protocol
    pub noise_proto: bool,
}

#[repr(C)]
pub struct RTypes_CryptoAuth2_Session_t {
    pub plaintext: *mut cffi::Iface_t,
    pub ciphertext: *mut cffi::Iface_t,
}

pub struct RTypes_Error_t {
    pub e: Option<anyhow::Error>,
}

#[repr(C)]
#[derive(Copy, Clone, PartialEq, Eq, Debug)]
pub enum RTypes_CryptoAuth2_TryHandshake_Code_t {
    ReplyToPeer,
    RecvPlaintext,
    Error,
    Done,
}

#[repr(C)]
pub struct RTypes_CryptoAuth2_TryHandshake_Ret_t {
    pub code: RTypes_CryptoAuth2_TryHandshake_Code_t,
    pub err: u32,
    pub sess: *mut RTypes_CryptoAuth2_Session_t,
    pub alloc: *mut cffi::Allocator_t,
}

pub struct RTypes_EventLoop_t {
    pub inner: Arc<EventLoop>,
    pub identity: Identity<Self>,
}

#[derive(PartialEq,Clone,Copy,IntoPrimitive,TryFromPrimitive)]
#[repr(i32)]
pub enum RTypes_SocketType {
    SendToFrames,
    Frames,
    Stream,
}

#[allow(dead_code)]
#[repr(C)]
pub struct RTypes_ExportMe {
    a: RTypes_IfWrapper_t,
    b: RTypes_StrList_t,
    c: RTypes_CryptoAuth_State_t,
    d: RTypes_CryptoStats_t,
    e: RTypes_CryptoAuth2_Session_t,
    f: *mut RTypes_Error_t,
    g: RTypes_CryptoAuth2_TryHandshake_Ret_t,
    h: RTypes_Seeder_DnsSeeds_t,
    i: *mut RTypes_EventLoop_t,
    j: RTypes_SocketType,
}
