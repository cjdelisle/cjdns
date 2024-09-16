use tokio::io::Interest;
use tokio::io::unix::AsyncFd;
use crate::cffi::Allocator_t;
use crate::gcl::Protected;
use crate::rffi::allocator::{self, file_line};
use crate::util::identity::{from_c, Identity};
use std::os::raw::{c_char, c_int, c_void};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;

struct FdReadable {
    _kill: tokio::sync::mpsc::UnboundedSender<()>,
    active: Arc<AtomicBool>,
}

pub struct Rffi_FdReadableTx{
    a: Arc<FdReadable>,
    identity: Identity<Self>,
}

pub extern "C" fn fd_readable_on_free(j: *mut c_void) {
    let timer_tx = unsafe { from_c!(j as *mut Rffi_FdReadableTx) };
    timer_tx.a.active.store(false, Ordering::Relaxed);
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
        let event_tx = allocator::adopt(alloc,
            Rffi_FdReadableTx{ a:rtx, identity: Identity::default() }
        );
        // Note: we must close the event in the allocator onFree rather than in the drop
        // because the drop only happens later, when Rust wants to.
        allocator::rs(alloc).on_free(
            fd_readable_on_free,
            event_tx as *mut c_void,
            file_line!()
        );
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