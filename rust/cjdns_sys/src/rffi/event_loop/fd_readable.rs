use tokio::io::Interest;
use tokio::io::unix::AsyncFd;
use crate::cffi::{Allocator_t, Allocator__onFree, Allocator_OnFreeJob};
use crate::gcl::Protected;
use crate::rffi::allocator;
use std::ffi::c_void;
use std::os::raw::{c_int, c_char};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;

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
    errout: *mut *const c_char,
    cb: unsafe extern "C" fn(*mut c_void),
    cb_context: *mut c_void,
    fd: c_int,
    alloc: *mut Allocator_t,
) {
    let fd = match AsyncFd::with_interest(fd, Interest::READABLE) {
        Ok(fd) => fd,
        Err(e) => {
            let out = allocator::adopt(alloc, format!("Error making AsyncFd: {e}"));
            let out = unsafe { (*out).as_bytes().as_ptr() } as *const c_char;
            unsafe {
                *errout = out;
            }
            return;
        }
    };

    let frc = Protected::new((cb, cb_context));

    let (kill, mut rx_kill) = tokio::sync::mpsc::unbounded_channel();
    let rtx = Arc::new(FdReadable {
        _kill: kill,
        active: Arc::new(AtomicBool::new(true)),
    });

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

    tokio::task::spawn(async move {
        loop {
            tokio::select! {
                r = fd.readable() => {
                    if !active.load(Ordering::Relaxed) {
                        log::info!("Rffi_pollFdReadable down: active = false");
                        break;
                    }
                    match r {
                        Ok(mut x) => {
                            let l = frc.lock();
                            unsafe { (l.0)(l.1); }
                            drop(l);
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