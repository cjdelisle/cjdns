use self::timeout::{Rffi_clearAllTimeouts, TimerTx};
use crate::cffi::{self,Allocator_t};
use crate::rffi::allocator;
use crate::gcl::GCL;
use parking_lot::ReentrantMutexGuard;
use std::os::raw::{c_uint,c_void};
use std::sync::atomic::{AtomicU32, Ordering};
use std::sync::{Mutex, Weak};
use std::sync::Arc;

mod process;
mod timeout;

/// The guard of an acquired [`GCL`].
pub struct Rffi_Glock_guard(ReentrantMutexGuard<'static, ()>);

/// Helper function to lock the Global C Lock, used only within libuv's core runtime (unix and windows).
#[no_mangle]
pub extern "C" fn Rffi_glock() -> *mut Rffi_Glock_guard {
    let guard = GCL.lock();
    Box::into_raw(Box::new(Rffi_Glock_guard(guard)))
}

/// Helper function to unlock the Global C Lock, as noted above.
#[no_mangle]
pub extern "C" fn Rffi_gunlock(guard: *mut Rffi_Glock_guard) {
    let _guard = unsafe { Box::from_raw(guard) };
    // let the guard go out of scope, be dropped, and unlock the Mutex.
}

const MAGIC: u64 = 0xde1dbeef_cafebabe;

/// A data repository, that groups objects related to this event loop.
pub struct EventLoop {
    /// Ref counter, to keep C's libuv running, when set to > 2**30, the loop is freed.
    ref_ctr: AtomicU32,
    /// Keep a loose track of all timers created, for clearAll.
    timers: Mutex<Vec<Weak<TimerTx>>>,
    /// Keep the event base from libuv so we can wake it up when needed
    base: *mut c_void,
}
unsafe impl Send for EventLoop {}
unsafe impl Sync for EventLoop {}
pub struct Rffi_EventLoop(Arc<EventLoop>, u64);
impl Rffi_EventLoop {
    fn arc_clone(&self) -> Arc<EventLoop> {
        assert!(self.1 == MAGIC);
        Arc::clone(&self.0)
    }
}

impl EventLoop {
    fn incr_ref(&self) -> u32 {
        self.ref_ctr.fetch_add(1, Ordering::Relaxed)
    }
    fn decr_ref(&self) -> u32 {
        let ret = self.ref_ctr.fetch_sub(1, Ordering::Relaxed);
        if !self.base.is_null() && ret < (1_u32<<30) {
            let _l = GCL.lock();
            unsafe { cffi::EventBase_wakeup(self.base) };
        }
        ret
    }
    fn curr_ref(&self) -> u32 {
        self.ref_ctr.load(Ordering::Relaxed)
    }
}

impl Drop for Rffi_EventLoop {
    fn drop(&mut self) {
        println!("Rffi_EventLoop DROP");
        self.0.ref_ctr.store(1_u32<<31, Ordering::Relaxed);
        Rffi_clearAllTimeouts(self)
    }
}

/// Create a new EventLoop data repository.
#[no_mangle]
pub extern "C" fn Rffi_mkEventLoop(alloc: *mut Allocator_t, base: *mut c_void) -> *mut Rffi_EventLoop {
    let data = Arc::new(EventLoop {
        ref_ctr: AtomicU32::new(0),
        timers: Mutex::new(vec![]),
        base,
    });
    allocator::adopt(alloc, Rffi_EventLoop(data, MAGIC))
}

/// Return some EventLoop's ref counter to C.
#[no_mangle]
pub extern "C" fn Rffi_eventLoopRefCtr(event_loop: *mut Rffi_EventLoop) -> c_uint {
    unsafe {
        assert!((&*event_loop).1 == MAGIC);
        &*event_loop
    }.0.curr_ref()
}
