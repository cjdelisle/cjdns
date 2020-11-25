//! Android wrapper
//!
//! Android VpnService is expect you to read/write packet from the tun device
//! file description opened by system process rather than in the cjd process,
//! this InterfaceWrapper handle this case.

#![allow(non_camel_case_types, non_snake_case)]

use std::ffi::c_void;

use crate::external::interface::iface::*;
use crate::external::memory::allocator::*;
use crate::external::util::identity::*;
use crate::external::util::log::*;
use crate::external::wire::error::*;
use crate::external::wire::message::*;

#[repr(C)]
pub struct AndroidWrapper {
    internalIf: Iface,
    externalIf: Iface,
}

#[repr(C)]
struct AndroidWrapper_pvt {
    public: AndroidWrapper,
    logger: *mut Log,
    identity: Identity,
}

extern "C" {
    fn AndroidWrapper_incomingFromWire(msg: *const Message, externalIf: *mut Iface) -> Error_s;
    fn AndroidWrapper_incomingFromUs(msg: *const Message, internalIf: *mut Iface) -> Error_s;
}

#[no_mangle]
pub extern "C" fn AndroidWrapper_new(alloc: *mut Allocator, log: *mut Log) -> *mut AndroidWrapper {
    let context = allocator_calloc_struct!(alloc, AndroidWrapper_pvt);
    Identity::set(context as *mut c_void);
    let p = unsafe { &mut (*context).public };
    p.externalIf.send = AndroidWrapper_incomingFromWire;
    p.internalIf.send = AndroidWrapper_incomingFromUs;
    unsafe {
        (*context).logger = log;
    }
    p as *mut AndroidWrapper
}
