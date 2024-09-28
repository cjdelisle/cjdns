use crate::cffi::{
    Allocator_t, Dict_t, Iface_t, Object_t, Sockaddr_t
};
use crate::external::interface::cif;
use crate::gcl::Protected;
use crate::interface::{
    socketiface::{SocketIface, SocketType},
    unixsocketiface::{UnixSocketClient, UnixSocketServer}
};
use crate::rffi::{allocator, benc};
use crate::rtypes::{RTypes_Error_t, RTypes_SocketType};
use crate::util::identity::from_c_const;
use crate::util::{
    callable::Callable,
    identity::{from_c, Identity},
    sockaddr::Sockaddr,
};
use std::sync::Arc;
use anyhow::anyhow;
use cjdns_bencode::BValue;
use libc::c_char;
use std::ffi::CStr;

#[no_mangle]
pub extern "C" fn Rffi_fileExists(
    existsOut: *mut bool,
    path: *const c_char,
    errorAlloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    // println!("Rffi_fileExists begin");
    let out = unsafe {
        if let Ok(str) = CStr::from_ptr(path).to_str() {
            if let Ok(md) = std::fs::metadata(str) {
                // println!("File {str} exists and is_file = {}", md.is_file());
                *existsOut = true;
            } else {
                *existsOut = false;
            }
            std::ptr::null_mut()
        } else {
            allocator::adopt(
                errorAlloc,
                RTypes_Error_t{
                    e: Some(anyhow::anyhow!("input did not decode as utf8")),
                },
            )
        }
    };
    // println!("Rffi_fileExists end");
    out
}

pub struct Rffi_SocketIface_t {
    si: SocketIface,
    identity: Identity<Self>,
}

#[no_mangle]
pub extern "C" fn Rffi_socketWorkerStates(
    outP: *mut *mut Object_t,
    si: *const Rffi_SocketIface_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let si = from_c_const!(si);
    let (sws, rws) = si.si.worker_states();

    let bv = BValue::builder()
        .set_dict()
        .add_dict_entry("send", |mut b|{
            b = b.set_dict();
            for (i, s) in sws.iter().enumerate() {
                b = b.add_dict_entry(i.to_string(), |b|b.set_str(format!("{s:?}")));
            }
            b
        })
        .add_dict_entry("recv", |mut b|{
            b = b.set_dict();
            for (i, r) in rws.iter().enumerate() {
                b = b.add_dict_entry(i.to_string(), |b|b.set_str(format!("{r:?}")));
            }
            b
        })
        .build();
    let out = benc::value_to_c(alloc, bv.inner());
    unsafe {
        *outP = out;
    }
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn Rffi_socketForFd(
    ifOut: *mut *mut Iface_t,
    so_out: *mut *mut Rffi_SocketIface_t,
    fd: libc::c_int,
    st: RTypes_SocketType,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut si = match SocketIface::new(vec![ fd ], st) {
        Ok(si) => si,
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t { e: Some(e) });
        }
    };
    let out = cif::wrap(alloc, &mut si.iface);
    let sout =
        allocator::adopt(alloc, Rffi_SocketIface_t{ si, identity: Default::default() });
    unsafe {
        *ifOut = out;
        *so_out = sout;
    }
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn Rffi_unixSocketConnect(
    ifOut: *mut *mut Iface_t,
    path: *const c_char,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut usc = if let Ok(path) = unsafe { CStr::from_ptr(path).to_str() } {
        match UnixSocketClient::new(path) {
            Ok(usc) => usc,
            Err(e) => {
                return allocator::adopt(alloc, RTypes_Error_t { e: Some(e) });
            }
        }
    } else {
        return allocator::adopt(alloc, RTypes_Error_t { e: Some(anyhow!("Failed to decode path as utf8")) });
    };
    let iface = cif::wrap(alloc, usc.iface());
    unsafe {
        *ifOut = iface;
    }
    allocator::adopt(alloc, usc);
    std::ptr::null_mut()
}

pub struct Rffi_SocketServer {
    server: UnixSocketServer,
    identity: Identity<Self>,
}

#[no_mangle]
pub extern "C" fn Rffi_unixSocketServerOnConnect(
    rss: *mut Rffi_SocketServer,
    f: extern "C" fn(*mut libc::c_void, *const Sockaddr_t),
    ctx: *mut libc::c_void,
) {
    let rss = from_c!(rss);
    let v: Option<Arc<dyn Callable<_,_>>> = if !ctx.is_null() {
        Some(Arc::new(<dyn Callable<_,_>>::new(Arc::new(Protected::new((f, ctx))), |fctx, sa: Sockaddr| {
            let mut a = allocator::new!();
            let sa_ptr = sa.c(a.c());
            let l = fctx.lock();
            (l.0)(l.1, sa_ptr);
        })))
    } else {
        None
    };
    tokio::task::spawn(rss.server.on_new_client(v));
}

#[no_mangle]
pub extern "C" fn Rffi_unixSocketServer(
    rssOut: *mut *mut Rffi_SocketServer,
    ifaceOut: *mut *mut Iface_t,
    path: *const c_char,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut usc = if let Ok(path) = unsafe { CStr::from_ptr(path).to_str() } {
        match UnixSocketServer::new(path) {
            Ok(usc) => usc,
            Err(e) => {
                return allocator::adopt(alloc, RTypes_Error_t { e: Some(e) });
            }
        }
    } else {
        return allocator::adopt(alloc, RTypes_Error_t { e: Some(anyhow!("Failed to decode path as utf8")) });
    };
    let iface_out = cif::wrap(alloc, &mut usc.iface);
    let out = allocator::adopt(alloc, Rffi_SocketServer{
        server: usc,
        identity: Default::default(),
    });
    unsafe {
        *rssOut = out;
        *ifaceOut = iface_out;
    }
    std::ptr::null_mut()
}