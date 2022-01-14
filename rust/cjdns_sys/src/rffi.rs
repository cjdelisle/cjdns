use crate::cffi::{Allocator_t, Iface_t};
use crate::external::interface::cif;
use std::ffi::{c_void, CStr};
use std::os::raw::c_char;
use std::sync::Once;
use std::time::{Duration, Instant, SystemTime};

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

/// Replaces libuv's function:
///
/// int uv_inet_ntop(int af, const void* src, char* dst, size_t size)
#[no_mangle]
pub unsafe extern "C" fn Rffi_inet_ntop(
    is_ip6: bool,
    addr: *const c_void,
    dst: *mut u8,
    dst_sz: u32,
) -> i32 {
    let ip_repr = if is_ip6 {
        let addr = addr as *const [u8; 16];
        std::net::Ipv6Addr::from(*addr).to_string()
    } else {
        let addr = addr as *const [u8; 4];
        std::net::Ipv4Addr::from(*addr).to_string()
    };
    if ip_repr.len() >= dst_sz as usize {
        return -1;
    }
    let dst = std::slice::from_raw_parts_mut(dst, dst_sz as usize);
    dst[..ip_repr.len()].copy_from_slice(ip_repr.as_bytes());
    dst[ip_repr.len()] = b'\0';
    0
}

/// Replaces libuv's function:
///
/// int uv_inet_pton(int af, const char* src, void* dst) {
#[no_mangle]
pub unsafe extern "C" fn Rffi_inet_pton(is_ip6: bool, src: *const c_char, addr: *mut u8) -> i32 {
    let src = CStr::from_ptr(src).to_string_lossy();
    let octets = if is_ip6 {
        match src.parse::<std::net::Ipv6Addr>() {
            Ok(addr) => addr.octets().to_vec(),
            Err(_) => return -1,
        }
    } else {
        match src.parse::<std::net::Ipv4Addr>() {
            Ok(addr) => addr.octets().to_vec(),
            Err(_) => return -1,
        }
    };
    let addr = std::slice::from_raw_parts_mut(addr, octets.len());
    addr.copy_from_slice(&octets);
    0
}

/// Non-monotonic nanosecond time, which has no relationship to any wall clock.
#[no_mangle]
pub unsafe extern "C" fn Rffi_hrtime() -> u64 {
    now_unix_epoch().as_nanos() as u64
}

fn now_unix_epoch() -> Duration {
    SystemTime::now()
        .duration_since(SystemTime::UNIX_EPOCH)
        .unwrap()
}

static mut BASE_INSTANT: Option<Instant> = None;
static mut INSTANT_OFFSET: u64 = 0;
static INIT: Once = Once::new();

/// Monotonic millisecond time.
#[no_mangle]
pub unsafe extern "C" fn Rffi_now_ms() -> u64 {
    INIT.call_once(|| {
        BASE_INSTANT = Some(Instant::now());
        INSTANT_OFFSET = now_unix_epoch().as_millis() as u64;
    });
    (Instant::now() - BASE_INSTANT.unwrap()).as_millis() as u64 + INSTANT_OFFSET
}
