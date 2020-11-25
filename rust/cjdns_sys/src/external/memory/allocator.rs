//! Memory allocator from C part of the project.

use std::ffi::c_void;
use std::os::raw::c_char;

/// External opaque C allocator type. Should be used in a form of a pointer only.
#[repr(C)]
pub struct Allocator {
    _dummy: usize,
}

extern "C" {
    pub fn Allocator__calloc(
        alloc: *mut Allocator,
        length: usize,
        count: usize,
        file_name: *const c_char,
        line_num: u32,
    ) -> *mut c_void;
}

/// Allocate some memory from this memory allocator.
/// The allocation will be aligned on the size of a pointer, if you need further alignment then
/// you must handle it manually.
/// Memory location will be filled with 0 bytes.
///
/// * `$alloc` the memory allocator.
/// * `$size` the number of bytes per element.
/// * `$count` the number of elements in the allocation.
/// * Returns a pointer to the newly allocated memory.
///
/// See also C `calloc()`.
macro_rules! allocator_calloc {
    ($alloc: expr, $length: expr, $count: expr) => {{
        use std::ffi::CString;
        let file_name = CString::new(file!())
            .expect("CString::new() failed")
            .as_ptr();
        let line_num = line!();
        unsafe {
            $crate::external::memory::allocator::Allocator__calloc(
                $alloc, $length, $count, file_name, line_num,
            )
        }
    }};
}

/// Allocate memory for the struct from this memory allocator.
/// The allocation will be aligned on the size of a pointer, if you need further alignment then
/// you must handle it manually.
/// Memory location will be filled with 0 bytes.
///
/// * `$alloc` the memory allocator.
/// * `$ty` the type (struct) that needs to be allocated.
/// * Returns a pointer to the newly allocated struct.
///
/// See also [`allocator_calloc`] macro.
macro_rules! allocator_calloc_struct {
    ($alloc: expr, $ty: ty) => {{
        let length = std::mem::size_of::<$ty>();
        let ptr = allocator_calloc!($alloc, length, 1);
        ptr as *mut $ty
    }};
}
