use self::timeout::TimerTx;
use crate::cffi::{self,Allocator_t};
use crate::rffi::{allocator, glock};
use crate::gcl::{self, GCL};
use crate::rtypes::RTypes_EventLoop_t;
use crate::util::identity::{from_c, Identity};
use parking_lot::{Mutex,ReentrantMutexGuard};
use std::os::raw::{c_uint,c_void};
use std::sync::atomic::{AtomicU32, Ordering};
use std::sync::{Arc,Weak};
use std::future::Future;
use tokio::sync::broadcast;

mod process;
mod timeout;
mod fd_readable;
mod udp;
mod unix_socket;

struct Quit {
    recv: broadcast::Receiver<()>,
    send: broadcast::Sender<()>,
}
impl Quit {
    fn new() -> Self {
        let (send, recv) = broadcast::channel(1);
        Self{send, recv}
    }
}

// Quit happens on free

/// A data repository, that groups objects related to this event loop.
pub struct EventLoop {
    /// Keep track of all timers created, for clearAll.
    timers: Mutex<Vec<Weak<TimerTx>>>,
    /// Abort when the 
    stop_loop: Mutex<Quit>,
}
impl Drop for EventLoop {
    fn drop(&mut self) {
        log::trace!("EventLoop DROP");
    }
}

#[no_mangle]
pub extern "C" fn Rffi_stopEventLoop(event_loop: *mut RTypes_EventLoop_t) {
    log::trace!("Rffi_stopEventLoop()");
    *from_c!(event_loop).inner.stop_loop.lock() = Quit::new();
}

#[no_mangle]
pub extern "C" fn Rffi_startEventLoop(event_loop: *mut RTypes_EventLoop_t) {
    log::trace!("Rffi_startEventLoop()");
    let mut quit_recv = from_c!(event_loop).inner.stop_loop.lock().recv.resubscribe();
    // This should be called with the c lock held because it's being called from C
    glock::Rffi_gunlock();
    let _ = quit_recv.blocking_recv();
}

/// Create a new EventLoop data repository.
#[no_mangle]
pub extern "C" fn Rffi_mkEventLoop(alloc: *mut Allocator_t) -> *mut RTypes_EventLoop_t {
    let inner = Arc::new(EventLoop{
        timers: Mutex::new(vec![]),
        stop_loop: Quit::new().into(),
    });
    allocator::adopt(alloc, RTypes_EventLoop_t{ inner, identity: Default::default() })
}