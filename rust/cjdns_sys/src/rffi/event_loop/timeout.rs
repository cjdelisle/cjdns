use super::{Rffi_EventLoop, GCL};
use crate::cffi::Allocator_t;
use crate::external::memory::allocator;
use std::ffi::c_void;
use std::os::raw::{c_int, c_ulong};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::time::Duration;

/// Used only internally, to send `*mut c_void` into a tokio task.
struct TimerCallback {
    cb: unsafe extern "C" fn(*mut c_void),
    cb_context: *mut c_void,
}

// SAFETY: this only holds if the C code is thread-safe, or the tokio Runtime uses only a single thread.
unsafe impl Send for TimerCallback {}

/// Commands to control a Timer task.
enum TimerCommand {
    Reset(c_ulong),
    Cancel,
}

/// The handle returned to C, used to talk to the timer task.
pub struct Rffi_TimerTx(Arc<TimerTx>);

/// Internal struct, which we keep weak references to.
pub struct TimerTx {
    tx: tokio::sync::mpsc::UnboundedSender<TimerCommand>,
    active: Arc<AtomicBool>,
    cb_int: u64,
}

/// Spawn a timer task for a timeout or interval, that calls some callback whenever it triggers.
#[no_mangle]
pub extern "C" fn Rffi_setTimeout(
    out_timer_tx: *mut *const Rffi_TimerTx,
    cb: unsafe extern "C" fn(*mut c_void),
    cb_context: *mut c_void,
    timeout_millis: c_ulong,
    repeat: bool,
    event_loop: *mut Rffi_EventLoop,
    alloc: *mut Allocator_t,
) {
    let cb_int = cb_context as u64;
    let tcb = TimerCallback { cb, cb_context };

    // it must be unbounded, since its Sender is sync, and can be used directly by the controller methods.
    let (tx, mut rx) = tokio::sync::mpsc::unbounded_channel();
    let rtx = Arc::new(TimerTx {
        tx,
        active: Arc::new(AtomicBool::new(true)),
        cb_int,
    });

    let event_loop = unsafe { &*event_loop };
    event_loop.timers.lock().unwrap().push(Arc::downgrade(&rtx));

    // do not clone rtx so we don't keep another tx around, so the "Allocator freed" detection works.
    // although with the new Drop impl for Rffi_EventLoop, the Cancel msg should get there first.
    let is_active = rtx.active.clone();

    unsafe {
        *out_timer_tx = allocator::adopt(alloc, Rffi_TimerTx(rtx));
    }

    event_loop.incr_ref();
    tokio::spawn(async move {
        let mut timeout_millis = timeout_millis;
        loop {
            tokio::select! {
                _ = tokio::time::sleep(Duration::from_millis(timeout_millis)) => {
                    let _guard = GCL.lock().unwrap();
                    if is_active.load(Ordering::Relaxed) {
                        unsafe { (tcb.cb)(tcb.cb_context); }
                    }
                    if !repeat {
                        break;
                    }
                }
                msg = rx.recv() => {
                    match msg {
                        Some(TimerCommand::Reset(new_timeout)) => {
                            println!("({:#x}) Received reset command: {} to {}",
                                cb_int, timeout_millis, new_timeout);
                            timeout_millis = new_timeout;
                        },
                        Some(TimerCommand::Cancel) => {
                            println!("({:#x}) Received cancel command", cb_int);
                            break
                        },
                        None => {
                            println!("({:#x}) Allocator freed, stopping timer task", cb_int);
                            break
                        },
                    }
                }
            }
        }
        is_active.store(false, Ordering::Relaxed);
        println!("({:#x}) timer task stopped", cb_int);
        event_loop.decr_ref();
    });
}

impl Drop for TimerTx {
    fn drop(&mut self) {
        self.active.store(false, Ordering::Relaxed);
        println!("({:#x}) the timer has been dropped", self.cb_int);
    }
}

impl TimerTx {
    /// Sends to the internal channel, converting the errors into an i32.
    fn rffi_send(&self, msg: TimerCommand) -> c_int {
        self.tx.send(msg).map_or(-1, |_| 0)
    }
}

/// Reset a timer task to change its timing.
#[no_mangle]
pub extern "C" fn Rffi_resetTimeout(
    timer_tx: *const Rffi_TimerTx,
    timeout_millis: c_ulong,
) -> c_int {
    let timer_tx = unsafe { &*timer_tx };
    assert!(
        timer_tx.0.active.load(Ordering::Relaxed),
        "cannot reset a cancelled timer"
    );
    timer_tx.0.rffi_send(TimerCommand::Reset(timeout_millis))
}

/// Cancel a timer task.
#[no_mangle]
pub extern "C" fn Rffi_clearTimeout(timer_tx: *const Rffi_TimerTx) -> c_int {
    let timer_tx = unsafe { &*timer_tx };
    timer_tx.0.active.store(false, Ordering::Relaxed);
    timer_tx.0.rffi_send(TimerCommand::Cancel)
}

/// Return 1 if a timer task is still running, 0 otherwise.
#[no_mangle]
pub extern "C" fn Rffi_isTimeoutActive(timer_tx: *const Rffi_TimerTx) -> c_int {
    let timer_tx = unsafe { &*timer_tx };
    timer_tx.0.active.load(Ordering::Relaxed) as _
}

/// Cancel all timer tasks.
#[no_mangle]
pub extern "C" fn Rffi_clearAllTimeouts(event_loop: *mut Rffi_EventLoop) {
    unsafe { &*event_loop }
        .timers
        .lock()
        .unwrap()
        .drain(..)
        .filter_map(|w| w.upgrade())
        .for_each(|timer_tx| {
            timer_tx.active.store(false, Ordering::Relaxed);
            timer_tx.rffi_send(TimerCommand::Cancel);
        })
}

#[cfg(test)]
mod tests {
    use super::super::Rffi_mkEventLoop;
    use super::*;
    use crate::external::memory::allocator::Allocator;

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_timer_interval() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.native);
        let mut timer = std::ptr::null();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const _ as _,
            1,
            true,
            event_loop,
            alloc.native,
        );

        // ensures the callback is being called repeatedly.
        for _ in 0..5 {
            rx.recv().unwrap();
        }

        let err = Rffi_resetTimeout(timer, 2);
        assert_eq!(err, 0);

        // ensures the callback is still being called.
        // we won't really test its accuracy here, since it's impossible to do it efficiently.
        for _ in 0..5 {
            rx.recv().unwrap();
        }

        let err = Rffi_clearTimeout(timer);
        assert_eq!(err, 0);

        // ensures the callback is not called anymore.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_timer_timeout() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.native);
        let mut timer = std::ptr::null();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const _ as _,
            1,
            false,
            event_loop,
            alloc.native,
        );

        // ensures the callback is called only once.
        rx.recv().unwrap();
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();

        let err = Rffi_resetTimeout(timer, 200);
        assert!(err < 0);

        let err = Rffi_clearTimeout(timer);
        assert!(err < 0);
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_timer_drop() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.native);
        let mut timer = std::ptr::null();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const _ as _,
            5,
            false,
            event_loop,
            alloc.native,
        );

        drop(alloc);

        // ensures the callback is never called.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_timer_clear_all() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.native);
        let start_timer = move |t, r, a: &Allocator| {
            let mut timer = std::ptr::null();
            Rffi_setTimeout(
                &mut timer as _,
                callback,
                &tx as *const _ as _,
                t,
                r,
                event_loop,
                a.native,
            );
        };

        start_timer(1, false, &alloc);
        start_timer(1, true, &alloc);
        start_timer(100, false, &alloc);
        start_timer(100, true, &alloc);

        // ensures the first two timers were already triggered.
        rx.recv().unwrap();
        rx.recv().unwrap();

        Rffi_clearAllTimeouts(event_loop);

        // ensures no callbacks are called anymore.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_timer_drop_all() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.native);
        let start_timer = move |t, r, a: &Allocator| {
            let mut timer = std::ptr::null();
            Rffi_setTimeout(
                &mut timer as _,
                callback,
                &tx as *const _ as _,
                t,
                r,
                event_loop,
                a.native,
            );
        };

        start_timer(1, false, &alloc);
        start_timer(1, true, &alloc);
        start_timer(100, false, &alloc);
        start_timer(100, true, &alloc);

        drop(alloc);

        // ensures no callbacks are called anymore.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }
}
