use eyre::eyre;

use crate::{
	cffi::{Allocator_t, Iface_t, Sockaddr_t, String_t}, external::interface::cif, interface::wsaddriface::WsAddrIface, rffi::{allocator, c_bail, cstr}, rtypes::RTypes_Error_t, util::{identity::Identity, sockaddr::Sockaddr}
};

#[repr(C)]
pub struct Rffi_WsIface {
    pvt: *mut Rffi_WsIface_pvt,
    iface: *mut Iface_t,
    local_addr: *mut Sockaddr_t,
}

pub struct Rffi_WsIface_pvt {
    ws: WsAddrIface,
    identity: Identity<Self>,
}

#[no_mangle]
pub extern "C" fn Rffi_wsIfaceNew(
    outp: *mut *mut Rffi_WsIface,
    bind_addr: *const Sockaddr_t,
    c_alloc: *mut Allocator_t,
    conn_timeout_secs: u32,
    peer_id: *const String_t,
) -> *mut RTypes_Error_t {
    let addr = if bind_addr.is_null() {
        "0.0.0.0:0".parse().unwrap()
    } else {
        Sockaddr::from(bind_addr).rs().unwrap()
    };
    let peer_id = match cstr(peer_id) {
        Some(peer_id) => peer_id,
        None => c_bail!(c_alloc, eyre!("peer_id must not be null")),
    };
    let peer_id = String::from_utf8_lossy(&peer_id[..]).to_string();
    log::info!("Binding WS socket: {addr}");
    let (ws, mut iface) = match WsAddrIface::new(&addr, conn_timeout_secs, peer_id) {
        Ok(uai) => uai,
        Err(e) => {
            return allocator::adopt(c_alloc, RTypes_Error_t{ e: Some(e) });
        }
    };
    log::info!("Bound WS socket: {}", &ws.local_addr);

    let local_addr = Sockaddr::from(&ws.local_addr).c(c_alloc);

    let iface = cif::wrap(c_alloc, &mut iface);

    let out = allocator::adopt(c_alloc, Rffi_WsIface{
        pvt: allocator::adopt(c_alloc, Rffi_WsIface_pvt{
            ws,
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
