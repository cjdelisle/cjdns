use super::str_to_c;
use crate::cffi::{self, Allocator_t};
use crate::external::interface::cif;
use crate::rffi::allocator;
use crate::rtypes::*;
use std::os::raw::{c_char, c_int};

#[no_mangle]
pub unsafe extern "C" fn Rffi_testwrapper_create(a: *mut Allocator_t) -> RTypes_IfWrapper_t {
    let w = crate::interface::rustiface_test_wrapper::TestWrapper::default();
    RTypes_IfWrapper_t {
        external: cif::wrap(a, w.ext),
        internal: cif::wrap(a, w.int),
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_android_create(a: *mut Allocator_t) -> RTypes_IfWrapper_t {
    let w = crate::interface::tuntap::android::AndroidWrapper::default();
    RTypes_IfWrapper_t {
        external: cif::wrap(a, w.ext),
        internal: cif::wrap(a, w.int),
    }
}

#[no_mangle]
pub static Rffi_CURRENT_PROTOCOL: usize =
    cffi::RBindings_Version::RBindings_Version_CurrentProtocol as usize;

#[no_mangle]
pub unsafe extern "C" fn Rffi_panic(msg: *const c_char) -> ! {
    panic!("{}", std::ffi::CStr::from_ptr(msg).to_string_lossy())
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_setLogger(l: *mut cffi::Log_t) {
    crate::cjdnslog::set_ffi_logger(l);
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_error(
    msg: *const c_char,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let s = std::ffi::CStr::from_ptr(msg).to_string_lossy();
    allocator::adopt(
        alloc,
        RTypes_Error_t {
            e: Some(anyhow::anyhow!(s)),
        },
    )
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_error_fl(
    msg: *const c_char,
    file: *const c_char,
    line: c_int,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let s = std::ffi::CStr::from_ptr(msg).to_string_lossy();
    let f = std::ffi::CStr::from_ptr(file).to_string_lossy();
    let ss = format!("{}:{}: {}", f, line, s);
    allocator::adopt(
        alloc,
        RTypes_Error_t {
            e: Some(anyhow::anyhow!(ss)),
        },
    )
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_printError(
    e: *mut RTypes_Error_t,
    alloc: *mut Allocator_t,
) -> *const c_char {
    e.as_ref()
        .map(|e| e.e.as_ref())
        .flatten()
        .map(|e| str_to_c(&format!("{:?}", e), alloc))
        .unwrap_or_else(std::ptr::null)
}
