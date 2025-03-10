use crate::cffi::Allocator_t;
use crate::gcl::Protected;
use crate::rffi::allocator::{self, file_line};
use crate::rtypes::RTypes_EventLoop_t;
use crate::util::identity::{from_c, from_c_const, Identity};
use std::ffi::c_void;
use std::os::raw::{c_int, c_ulong};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::time::Duration;

/// Commands to control a Timer task.
enum TimerCommand {
    Reset(c_ulong),
    Cancel,
    Free,
}

/// The handle returned to C, used to talk to the timer task.
pub struct Rffi_TimerTx{
    a: Arc<TimerTx>,
    identity: Identity<Self>,
}

/// Internal struct, which we keep weak references to.
pub struct TimerTx {
    tx: tokio::sync::mpsc::UnboundedSender<TimerCommand>,
    active: Arc<AtomicBool>,
}

pub extern "C" fn timeout_on_free(j: *mut c_void) {
    let timer_tx = from_c!(j as *mut Rffi_TimerTx);
    timer_tx.a.active.store(false, Ordering::Relaxed);
    timer_tx.a.rffi_send(TimerCommand::Free);
}

/// Spawn a timer task for a timeout or interval, that calls some callback whenever it triggers.
#[no_mangle]
pub extern "C" fn Rffi_setTimeout(
    out_timer_tx: *mut *mut Rffi_TimerTx,
    cb: unsafe extern "C" fn(*mut c_void),
    cb_context: *mut c_void,
    timeout_millis: c_ulong,
    repeat: bool,
    event_loop: *mut RTypes_EventLoop_t,
    alloc: *mut Allocator_t,
) {
    let cb_int = cb_context as u64;
    let tcb = Protected::new((cb, cb_context));

    // it must be unbounded, since its Sender is sync, and can be used directly by the controller methods.
    let (tx, mut rx) = tokio::sync::mpsc::unbounded_channel();
    let rtx = Arc::new(TimerTx {
        tx,
        active: Arc::new(AtomicBool::new(true)),
    });

    from_c!(event_loop).inner.timers.lock().push(Arc::downgrade(&rtx));

    // do not clone rtx so we don't keep another tx around, so the "Allocator freed" detection works.
    // although with the new Drop impl for RTypes_EventLoop, the Cancel msg should get there first.
    let is_active = rtx.active.clone();

    unsafe {
        let timer_tx = allocator::adopt(alloc, Rffi_TimerTx{
            a: rtx,
            identity: Default::default(),
        });
        // Note: we must close the event in the allocator onFree rather than in the drop
        // because the drop only happens later, when Rust wants to.
        allocator::rs(alloc).on_free(timeout_on_free, timer_tx as _, file_line!());
        *out_timer_tx = timer_tx;
    }

    tokio::task::spawn(async move {
        let mut timeout_millis = timeout_millis;
        loop {
            tokio::select! {
                _ = tokio::time::sleep(Duration::from_millis(timeout_millis.into())) => {
                    let l = tcb.lock();
                    if is_active.load(Ordering::Relaxed) {
                        unsafe { (l.0)(l.1); }
                    }
                    if !repeat {
                        break;
                    }
                }
                mut msg = rx.recv() => {
                    if loop {
                        match msg {
                            Some(TimerCommand::Reset(new_timeout)) => {
                                println!("({:#x}) Received reset command: {} to {}",
                                    cb_int, timeout_millis, new_timeout);
                                timeout_millis = new_timeout;
                                break false;
                            },
                            Some(TimerCommand::Cancel) => {
                                println!("({:#x}) Received cancel command", cb_int);
                                msg = rx.recv().await;
                            },
                            Some(TimerCommand::Free) |
                            None => {
                                //println!("({:#x}) Allocator freed, stopping timer task", cb_int);
                                break true;
                            },
                        }
                    } { break; }
                }
            }
        }
        is_active.store(false, Ordering::Relaxed);
        //println!("({:#x}) timer task stopped", cb_int);
    });
}

impl Drop for TimerTx {
    fn drop(&mut self) {
        self.active.store(false, Ordering::Relaxed);
        //println!("({:#x}) the timer has been dropped", self.cb_int);
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
    let timer_tx = from_c_const!(timer_tx);
    timer_tx.a.rffi_send(TimerCommand::Reset(timeout_millis))
}

/// Cancel a timer task.
#[no_mangle]
pub extern "C" fn Rffi_clearTimeout(timer_tx: *const Rffi_TimerTx) -> c_int {
    let timer_tx = from_c_const!(timer_tx);
    timer_tx.a.active.store(false, Ordering::Relaxed);
    timer_tx.a.rffi_send(TimerCommand::Cancel)
}

/// Return 1 if a timer task is still running, 0 otherwise.
#[no_mangle]
pub extern "C" fn Rffi_isTimeoutActive(timer_tx: *const Rffi_TimerTx) -> c_int {
    let timer_tx = from_c_const!(timer_tx);
    timer_tx.a.active.load(Ordering::Relaxed) as _
}

/// Cancel all timer tasks.
#[no_mangle]
pub extern "C" fn Rffi_clearAllTimeouts(event_loop: *mut RTypes_EventLoop_t) {
    from_c!(event_loop)
        .inner
        .timers
        .lock()
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
    use crate::rffi::allocator::Allocator;

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    #[ignore = "Segfaulting on EventLoop_wakeup"]
    async fn test_timer_interval() {
        let mut alloc = allocator::new!();

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.c());
        let mut timer = std::ptr::null_mut();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const _ as _,
            1,
            true,
            event_loop,
            alloc.c(),
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
    #[ignore = "Segfaulting on EventLoop_wakeup"]
    async fn test_timer_timeout() {
        let mut alloc = allocator::new!();

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.c());
        let mut timer = std::ptr::null_mut();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const _ as _,
            1,
            false,
            event_loop,
            alloc.c(),
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
    #[ignore = "Segfaulting on EventLoop_wakeup"]
    async fn test_timer_drop() {
        let mut alloc = allocator::new!();

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.c());
        let mut timer = std::ptr::null_mut();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const _ as _,
            5,
            false,
            event_loop,
            alloc.c(),
        );

        drop(alloc);

        // ensures the callback is never called.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    #[ignore = "Segfaulting on EventLoop_wakeup"]
    async fn test_timer_clear_all() {
        let mut alloc = allocator::new!();

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.c());
        let start_timer = move |t, r, a: &mut Allocator| {
            let mut timer = std::ptr::null_mut();
            Rffi_setTimeout(
                &mut timer as _,
                callback,
                &tx as *const _ as _,
                t,
                r,
                event_loop,
                a.c(),
            );
        };

        start_timer(1, false, &mut alloc);
        start_timer(1, true, &mut alloc);
        start_timer(100, false, &mut alloc);
        start_timer(100, true, &mut alloc);

        // ensures the first two timers were already triggered.
        rx.recv().unwrap();
        rx.recv().unwrap();

        Rffi_clearAllTimeouts(event_loop);

        // ensures no callbacks are called anymore.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    #[ignore = "Segfaulting on EventLoop_wakeup"]
    async fn test_timer_drop_all() {
        let mut alloc = allocator::new!();

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let event_loop = Rffi_mkEventLoop(alloc.c());
        let start_timer = move |t, r, a: &mut Allocator| {
            let mut timer = std::ptr::null_mut();
            Rffi_setTimeout(
                &mut timer as _,
                callback,
                &tx as *const _ as _,
                t,
                r,
                event_loop,
                a.c(),
            );
        };

        start_timer(1, false, &mut alloc);
        start_timer(1, true, &mut alloc);
        start_timer(100, false, &mut alloc);
        start_timer(100, true, &mut alloc);

        drop(alloc);

        // ensures no callbacks are called anymore.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }
}
