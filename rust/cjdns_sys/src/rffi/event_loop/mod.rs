use self::timeout::{Rffi_clearAllTimeouts, TimerTx};
use crate::cffi::Allocator_t;
use crate::external::memory::allocator;
use once_cell::sync::Lazy;
use std::os::raw::c_uint;
use std::sync::atomic::{AtomicU32, Ordering};
use std::sync::{Mutex, MutexGuard, Weak};

mod process;
mod timeout;

/// Global C lock, to make callbacks into C, while keeping libuv's and tokio's async Runtimes synced.
static GCL: Lazy<Mutex<()>> = Lazy::new(|| Mutex::new(()));

/// The guard of an acquired [`GCL`].
pub struct Rffi_Glock_guard(MutexGuard<'static, ()>);

/// Helper function to lock the Global C Lock, used only within libuv's core runtime (unix and windows).
#[no_mangle]
pub extern "C" fn Rffi_glock() -> *mut Rffi_Glock_guard {
    let guard = GCL.lock().unwrap();
    Box::into_raw(Box::new(Rffi_Glock_guard(guard)))
}

/// Helper function to unlock the Global C Lock, as noted above.
#[no_mangle]
pub extern "C" fn Rffi_gunlock(guard: *mut Rffi_Glock_guard) {
    let _guard = unsafe { Box::from_raw(guard) };
    // let the guard go out of scope, be dropped, and unlock the Mutex.
}

/// A data repository, that groups objects related to this event loop.
pub struct Rffi_EventLoop {
    /// Ref counter, to keep C's libuv running.
    ref_ctr: AtomicU32,
    /// Keep a loose track of all timers created, for clearAll.
    timers: Mutex<Vec<Weak<TimerTx>>>,
}

impl Rffi_EventLoop {
    fn incr_ref(&self) -> u32 {
        self.ref_ctr.fetch_add(1, Ordering::Relaxed)
    }
    fn decr_ref(&self) -> u32 {
        self.ref_ctr.fetch_sub(1, Ordering::Relaxed)
    }
    fn curr_ref(&self) -> u32 {
        self.ref_ctr.load(Ordering::Relaxed)
    }
}

impl Drop for Rffi_EventLoop {
    fn drop(&mut self) {
        Rffi_clearAllTimeouts(self)
    }
}

/// Create a new EventLoop data repository.
#[no_mangle]
pub extern "C" fn Rffi_mkEventLoop(alloc: *mut Allocator_t) -> *mut Rffi_EventLoop {
    let data = Rffi_EventLoop {
        ref_ctr: AtomicU32::new(0),
        timers: Mutex::new(vec![]),
    };
    allocator::adopt(alloc, data)
}

/// Return some EventLoop's ref counter to C.
#[no_mangle]
pub extern "C" fn Rffi_eventLoopRefCtr(event_loop: *mut Rffi_EventLoop) -> c_uint {
    unsafe { &*event_loop }.curr_ref()
}
