use crate::cffi::{Allocator_t, Sockaddr_t, Iface_t};
use crate::external::interface::cif;
use crate::rffi::allocator;
use crate::rtypes::RTypes_Error_t;
use std::os::raw::c_char;
use crate::util::sockaddr::Sockaddr;
use crate::interface::udpaddriface::UDPAddrIface;
use crate::util::identity::{Identity,from_c};

#[repr(C)]
pub struct Rffi_UDPIface {
    pvt: *mut Rffi_UDPIface_pvt,
    iface: *mut Iface_t,
    local_addr: *mut Sockaddr_t,
}

pub struct Rffi_UDPIface_pvt {
    udp: UDPAddrIface,
    identity: Identity<Self>,
}

#[no_mangle]
pub extern "C" fn Rffi_udpIfaceGetFd(iface: *mut Rffi_UDPIface_pvt) -> i32 {
    from_c!(iface).udp.get_fd()
}

#[no_mangle]
pub extern "C" fn Rffi_udpIfaceSetBroadcast(iface: *mut Rffi_UDPIface_pvt, broadcast: bool) -> i32 {
    match from_c!(iface).udp.set_broadcast(broadcast) {
        Ok(()) => 0,
        Err(e) => {
            log::info!("Unable to set broadcast on UDPInterface: {e}");
            -1
        }
    }
}

#[no_mangle]
pub extern "C" fn Rffi_udpIfaceSetDscp(iface: *mut Rffi_UDPIface_pvt, dscp: u8) -> i32 {
    match from_c!(iface).udp.set_dscp(dscp) {
        Ok(()) => 0,
        Err(e) => {
            log::info!("Unable to set dscp on UDPInterface: {e}");
            -1
        }
    }
}

#[no_mangle]
pub extern "C" fn Rffi_udpIfaceNew(
    outp: *mut *mut Rffi_UDPIface,
    bind_addr: *const Sockaddr_t,
    c_alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let addr = if bind_addr.is_null() {
        "0.0.0.0:0".parse().unwrap()
    } else {
        Sockaddr::from(bind_addr).rs().unwrap()
    };
    log::info!("Binding UDP socket: {addr}");
    let (udp, mut iface) = match UDPAddrIface::new(&addr) {
        Ok(uai) => uai,
        Err(e) => {
            return allocator::adopt(c_alloc, RTypes_Error_t{ e: Some(e) });
        }
    };
    log::info!("Bound UDP socket: {}", &udp.local_addr);

    let local_addr = Sockaddr::from(&udp.local_addr).c(c_alloc);

    let iface = cif::wrap(c_alloc, &mut iface);

    let out = allocator::adopt(c_alloc, Rffi_UDPIface{
        pvt: allocator::adopt(c_alloc, Rffi_UDPIface_pvt{
            udp,
            identity: Default::default(),
        }),
        iface,
        local_addr,
    });

    unsafe {
        *outp = out;
    }
    std::ptr::null_mut()
}