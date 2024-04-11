use super::{Rffi_EventLoop, GCL};
use crate::cffi::{Allocator_t, Allocator__onFree, Allocator_OnFreeJob};
use crate::rffi::allocator;
use std::ffi::c_void;
use std::os::raw::c_int;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;

/// Used only internally, to send `*mut c_void` into a tokio task.
struct FdReadableCallback {
    cb: unsafe extern "C" fn(*mut c_void),
    cb_context: *mut c_void,
}

// SAFETY: this only holds if the C code is thread-safe, or the tokio Runtime uses only a single thread.
unsafe impl Send for FdReadableCallback {}

struct FdReadable {
    _kill: tokio::sync::mpsc::UnboundedSender<()>,
    active: Arc<AtomicBool>,
}

pub struct Rffi_FdReadableTx(Arc<FdReadable>);

pub extern "C" fn fd_readable_on_free(j: *mut Allocator_OnFreeJob) -> c_int {
    let timer_tx = unsafe { &*((*j).userData as *mut Rffi_FdReadableTx) };
    timer_tx.0.active.store(false, Ordering::Relaxed);
    0
}

#[no_mangle]
pub extern "C" fn Rffi_pollFdReadable(
    out: *mut *mut Rffi_FdReadableTx,
    cb: unsafe extern "C" fn(*mut c_void),
    cb_context: *mut c_void,
    fd: c_int,
    event_loop: *mut Rffi_EventLoop,
    alloc: *mut Allocator_t,
) {
    // let cb_int = cb_context as u64;
    let frc = FdReadableCallback{ cb, cb_context };

    let (kill, mut rx_kill) = tokio::sync::mpsc::unbounded_channel();
    let rtx = Arc::new(FdReadable {
        _kill: kill,
        active: Arc::new(AtomicBool::new(true)),
    });

    let event_loop = unsafe { (&*event_loop).arc_clone() };
    let active = Arc::clone(&rtx.active);

    unsafe {
        let event_tx = allocator::adopt(alloc, Rffi_FdReadableTx(rtx));
        // Note: we must close the event in the allocator onFree rather than in the drop
        // because the drop only happens later, when Rust wants to.
        Allocator__onFree(alloc,
            Some(fd_readable_on_free),
            event_tx as *mut c_void,
            ("fd_readable.rs\0").as_bytes().as_ptr() as *const ::std::os::raw::c_char,
            line!() as std::os::raw::c_int);
        *out = event_tx;
    }

    event_loop.arc_clone().event_job(async move {
        let fd = tokio::io::unix::AsyncFd::new(fd).unwrap();
        loop {
            tokio::select! {
                r = fd.readable() => {
                    if !active.load(Ordering::Relaxed) {
                        log::info!("Rffi_pollFdReadable down: active = false");
                        break;
                    }
                    match r {
                        Ok(mut x) => {
                            let guard = GCL.lock();
                            unsafe { (frc.cb)(frc.cb_context); }
                            drop(guard);
                            x.clear_ready();
                        }
                        Err(e) => {
                            log::debug!("Rffi_pollFdReadable returned error: {e} - continuing");
                        }
                    }
                }
                _ = rx_kill.recv() => {
                    log::info!("Rffi_pollFdReadable down: rx_kill received");
                    break;
                }
            }
        }
    });
}