use self::timeout::TimerTx;
use crate::cffi::{self,Allocator_t};
use crate::rffi::allocator;
use crate::gcl::GCL;
use parking_lot::{Mutex,ReentrantMutexGuard};
use std::os::raw::{c_uint,c_void};
use std::sync::atomic::{AtomicU32, Ordering};
use std::sync::{Arc,Weak};
use std::future::Future;
use tokio::sync::broadcast;

mod process;
mod timeout;
mod fd_readable;

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
    quit: broadcast::Receiver<()>,
    quitter: Mutex<Option<broadcast::Sender<()>>>,
    /// Keep the event base from libuv so we can wake it up when needed
    base: *mut c_void,
}
unsafe impl Send for EventLoop {}
unsafe impl Sync for EventLoop {}
impl Drop for EventLoop {
    fn drop(&mut self) {
        log::trace!("EventLoop DROP");
    }
}
pub struct Rffi_EventLoop(Arc<EventLoop>, u64);
impl Rffi_EventLoop {
    fn arc_clone(&self) -> Rffi_EventLoop {
        assert!(self.1 == MAGIC);
        Rffi_EventLoop(Arc::clone(&self.0), MAGIC)
    }
    pub fn event_job<T>(&self, future: T) -> tokio::task::JoinHandle<()>
    where
        T: Future + Send + 'static,
    {
        let evl = self.arc_clone();
        let mut quit = evl.0.quit.resubscribe();
        evl.incr_ref();
        tokio::spawn(async move {
            tokio::select! {
                _ = future => {},
                _ = quit.recv() => {},
            }
            evl.decr_ref();
        })
    }
    fn incr_ref(&self) -> u32 {
        self.0.ref_ctr.fetch_add(1, Ordering::SeqCst)
    }
    fn decr_ref(&self) -> u32 {
        let _l = GCL.lock();
        let ret = self.0.ref_ctr.fetch_sub(1, Ordering::SeqCst);
        unsafe { cffi::EventBase_wakeup(self.0.base) };
        ret
    }
    fn curr_ref(&self) -> u32 {
        self.0.ref_ctr.load(Ordering::SeqCst)
    }
}

#[no_mangle]
pub extern "C" fn Rffi_stopEventLoop(event_loop: *mut Rffi_EventLoop) {
    log::trace!("Rffi_stopEventLoop()");
    let el = unsafe { (&*event_loop).arc_clone() };
    let mut qg_l = el.0.quitter.lock();
    let qg = qg_l.take();
    drop(qg);
}

/// Create a new EventLoop data repository.
#[no_mangle]
pub extern "C" fn Rffi_mkEventLoop<'a>(alloc: *mut Allocator_t, base: *mut c_void) -> *mut Rffi_EventLoop {
    let (quitter, quit) = broadcast::channel(1);
    let data = Arc::new(EventLoop{
        ref_ctr: AtomicU32::new(0),
        timers: Mutex::new(vec![]),
        base,
        quit,
        quitter: Mutex::new(Some(quitter)),
    });
    allocator::adopt(alloc, Rffi_EventLoop(data, MAGIC))
}

/// Return some EventLoop's ref counter to C.
#[no_mangle]
pub extern "C" fn Rffi_eventLoopRefCtr(event_loop: *mut Rffi_EventLoop) -> c_uint {
    unsafe {
        assert!((&*event_loop).1 == MAGIC);
        &*event_loop
    }.curr_ref()
}
