//!
//! This whole module is used to generate Rffi.h using cbindgen.
//!

#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

mod crypto;
mod event_loop;
mod network;
mod time;
mod util;
mod glock;
mod base10;
pub mod allocator;
mod seeder;

use anyhow::{bail, Result};

use crate::bytestring::ByteString;
use crate::cffi::{Allocator_t, String_t};
use std::os::raw::c_char;

unsafe fn cu8(s: *const u8, len: usize) -> Vec<u8> {
    std::slice::from_raw_parts::<u8>(s, len).to_vec()
}

fn cstr(s: *const String_t) -> Option<ByteString> {
    if s.is_null() {
        None
    } else {
        let bytes = unsafe { cu8((*s).bytes as *const u8, (*s).len) };
        Some(ByteString(bytes))
    }
}

fn strc(alloc: *mut Allocator_t, s: ByteString) -> *mut String_t {
    let ByteString(mut s) = s;
    let len = s.len();
    let bytes = s.as_mut_ptr() as *mut c_char;
    allocator::adopt(alloc, s);
    allocator::adopt(alloc, String_t { len, bytes })
}

fn str_to_c(s: &str, alloc: *mut Allocator_t) -> *mut c_char {
    let c_str = std::ffi::CString::new(s).unwrap();
    let adopted = allocator::adopt(alloc, c_str);
    unsafe { (*adopted).as_ptr() as _ }
}

fn cstr_to_string(c: *const String_t) -> Result<String> {
    if let Some(s) = cstr(c) {
        match String::from_utf8(s.0) {
            Ok(s) => Ok(s),
            Err(_) => {
                bail!("cstr_to_string() not valid UTF-8");
            }
        }
    } else {
        bail!("cstr_to_string() was NULL");
    }
}

macro_rules! c_error {
    ($alloc:expr, $may_err:expr) => {
        match $may_err {
            Ok(s) => s,
            Err(e) => {
                return allocator::adopt(
                    $alloc,
                    crate::rtypes::RTypes_Error_t { e: Some(e) }
                );
            }
        }
    };
}
pub(crate) use c_error;