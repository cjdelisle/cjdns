use crate::cffi::{
    Allocator_t,
    Iface_t,
    Sockaddr_t
};
use crate::external::interface::cif;
use crate::gcl;
use crate::interface::{
    socketiface::{SocketIface, SocketType},
    unixsocketiface::{UnixSocketClient, UnixSocketServer}
};
use crate::rffi::allocator;
use crate::rtypes::RTypes_Error_t;
use crate::util::{
    callable::{self, Callable},
    identity::{from_c, Identity},
    sockaddr::Sockaddr,
};
use std::sync::Arc;
use anyhow::anyhow;
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

#[no_mangle]
pub extern "C" fn Rffi_socketForFd(
    ifOut: *mut *mut Iface_t,
    fd: libc::c_int,
    socket_type: libc::c_int,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let st = match socket_type {
        0 => SocketType::Stream,
        1 => SocketType::Frames,
        2 => SocketType::SendToFrames,
        _ => {
            return allocator::adopt(alloc, RTypes_Error_t {
                e: Some(anyhow::anyhow!("Invalid socket type: {socket_type}")),
            });
        }
    };
    let mut si = match SocketIface::new(vec![ fd ], st) {
        Ok(si) => si,
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t { e: Some(e) });
        }
    };
    let out = cif::wrap(alloc, &mut si.iface);
    allocator::adopt(alloc, si);
    unsafe {
        *ifOut = out;
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
    #[derive(Clone)]
    struct FuncAndCtx {
        f: extern "C" fn(*mut libc::c_void, *const Sockaddr_t),
        ctx: *mut libc::c_void,
    }
    unsafe impl Send for FuncAndCtx {}
    unsafe impl Sync for FuncAndCtx {}
    let v: Option<Arc<dyn Callable<_,_>>> = if !ctx.is_null() {
        Some(Arc::new(callable::new(FuncAndCtx{f, ctx}, |fctx, sa: Sockaddr| async move {
            let mut a = allocator::new!();
            let sa_ptr = sa.c(a.c());
            let _l = gcl::GCL.lock();
            (fctx.f)(fctx.ctx, sa_ptr);
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