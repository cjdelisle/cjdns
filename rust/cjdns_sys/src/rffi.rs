use crate::cffi::{Allocator_t, Iface_t};
use crate::external::interface::cif;

// This file is used to generate cbindings.h using cbindgen

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct Rffi_IfWrapper_t {
    pub internal: *mut Iface_t,
    pub external: *mut Iface_t,
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_testwrapper_create(a: *mut Allocator_t) -> Rffi_IfWrapper_t {
    let w = crate::interface::rustiface_test_wrapper::TestWrapper::default();
    Rffi_IfWrapper_t {
        external: cif::wrap(a, w.ext),
        internal: cif::wrap(a, w.int),
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_android_create(a: *mut Allocator_t) -> Rffi_IfWrapper_t {
    let w = crate::interface::tuntap::android::AndroidWrapper::default();
    Rffi_IfWrapper_t {
        external: cif::wrap(a, w.ext),
        internal: cif::wrap(a, w.int),
    }
}
