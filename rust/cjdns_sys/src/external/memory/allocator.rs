//! Memory allocator from C part of the project.

use crate::cffi::{
    Allocator_t,
    Allocator_OnFreeJob,
    Allocator__onFree,
    Allocator__free,
    MallocAllocator__new,
    Allocator__child,
};
use std::any::Any;
use std::os::raw::{c_char, c_int, c_void, c_ulong};

unsafe extern "C" fn drop_on_free(job_p: *mut Allocator_OnFreeJob) -> c_int {
    let job = job_p.as_ref().unwrap();
    drop(Box::from_raw(job.userData as *mut Box<dyn std::any::Any>));
    0
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
pub fn adopt<T: 'static>(alloc: *mut Allocator_t, t: T) -> &'static mut T {
    let bat: Box<dyn Any> = Box::new(t);
    let out: *mut Box<dyn Any> = Box::into_raw(Box::new(bat));
    unsafe {
        Allocator__onFree(
            alloc,
            Some(drop_on_free),
            out as *mut c_void,
            b"<rust>\0".as_ptr() as *const c_char,
            0,
        );
        out.as_mut().unwrap().downcast_mut().unwrap()
    }
}

const NAME: *const c_char = b"allocator.rs\0".as_ptr() as *const c_char;

pub struct Allocator{
    pub native: *mut Allocator_t,
    pub owned: bool,
}
impl Allocator {
    pub fn wrap(native: *mut Allocator_t) -> Allocator {
        Allocator{ native, owned: false }
    }
    pub fn new(max_alloc: usize) -> Allocator {
        let native = unsafe { MallocAllocator__new(max_alloc as c_ulong, NAME, line!() as i32) };
        Allocator{ native, owned: true }
    }
    pub fn adopt<T: 'static>(&self, t: T) -> &'static mut T {
        adopt(self.native, t)
    }
    pub fn child(&self) -> Allocator {
        Allocator{ native: unsafe { Allocator__child(self.native, NAME, -1) }, owned: false }
    }
}
impl Drop for Allocator {
    fn drop(&mut self) {
        if self.owned {
            unsafe { Allocator__free(self.native, NAME, -1) };
        }
    }
}