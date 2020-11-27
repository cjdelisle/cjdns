//! Memory allocator from C part of the project.

/// External opaque C allocator type. Should be used in a form of a pointer only.
use crate::ffi::Allocator;

/// TODO: This needs to hook Allocator_onFree() in order to
/// un-leak the memory when the allocator is freed up
pub fn adopt<T: 'static>(_alloc: *mut Allocator, t: T) -> &'static mut T {
    Box::leak(Box::new(t))
}
