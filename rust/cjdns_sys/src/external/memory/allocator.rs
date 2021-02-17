//! Memory allocator from C part of the project.

use crate::cffi::{Allocator, Allocator_OnFreeJob, Allocator__onFree};
use std::any::Any;
use std::os::raw::{c_char, c_int, c_void};

unsafe extern "C" fn drop_on_free(job_p: *mut Allocator_OnFreeJob) -> c_int {
    let job = job_p.as_ref().unwrap();
    Box::from_raw(job.userData as *mut Box<dyn std::any::Any>);
    0 as c_int
}

/// "Adopt" a structure in Rust and transfer logical ownership to an Allocator.
/// The structure that is adopted is placed in a box which is then leaked
/// so that rust will nolonger care about the memory. But Allocator__onFree()
/// is called to setup a callback when the allocator is freed.
/// When the onFree callback is called, this code re-constructs the leaked box
/// and then drops it, causing Rust to properly drop all of the memory which
/// is related to the adopted structure, including any Arc, String, Vec or other
/// heap objects which are owned by it.
#[allow(clippy::not_unsafe_ptr_arg_deref)]
pub fn adopt<T: 'static>(alloc: *mut Allocator, t: T) -> &'static mut T {
    let bat: Box<dyn Any> = Box::new(t);
    let out: *mut Box<dyn Any> = Box::into_raw(Box::new(bat));
    unsafe {
        Allocator__onFree(
            alloc,
            Some(drop_on_free),
            out as *mut c_void,
            b"<rust>\0".as_ptr() as *const c_char,
            0 as c_int,
        );
        out.as_mut().unwrap().downcast_mut().unwrap()
    }
}
