use once_cell::sync::Lazy;
use parking_lot::{ReentrantMutex,ReentrantMutexGuard};
use std::ops::Deref;
use std::cell::RefCell;

use crate::gcl::GCL;

thread_local! {
    static GCL_HELD: RefCell<Option<ReentrantMutexGuard<'static, ()>>> = RefCell::new(None);
}

#[no_mangle]
pub extern "C" fn Rffi_glock() {
    GCL_HELD.with_borrow_mut(|l|{
        if l.is_none() {
            *l = Some(GCL.lock());
        }
    })
}

#[no_mangle]
pub extern "C" fn Rffi_gunlock() {
    GCL_HELD.with_borrow_mut(|l|{
        drop(l.take());
    })
}