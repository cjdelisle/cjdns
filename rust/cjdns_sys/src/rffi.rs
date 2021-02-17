#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

use std::convert::TryFrom;
use std::os::raw::{c_char, c_int};
use std::sync::Arc;

use crate::bytestring::ByteString;
use crate::cffi::{self, Allocator_t, Message_t, Random_t, String_t};
use crate::crypto::crypto_auth;
use crate::crypto::keys::{IpV6, PrivateKey, PublicKey};
use crate::external::interface::cif;
use crate::external::memory::allocator;
use crate::interface::wire::message::Message;
use crate::rtypes::*;

// This file is used to generate cbindings.h using cbindgen

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

pub struct Rffi_CryptoAuth2_t(Arc<crypto_auth::CryptoAuth>);
pub struct Rffi_CryptoAuth2_Session_t {
    s: Arc<crypto_auth::Session>,
    pub plaintext: *mut cffi::Iface_t,
    pub ciphertext: *mut cffi::Iface_t,
}

unsafe fn cu8(s: *const u8, len: usize) -> Vec<u8> {
    std::slice::from_raw_parts::<u8>(s, len).to_vec()
}

unsafe fn cstr(s: *const String_t) -> Option<ByteString> {
    if s.is_null() {
        None
    } else {
        let bytes = cu8((*s).bytes as *const u8, (*s).len);
        Some(ByteString(bytes))
    }
}

unsafe fn strc(alloc: *mut Allocator_t, s: ByteString) -> *mut String_t {
    let ByteString(mut s) = s;
    let len = s.len();
    let bytes = s.as_mut_ptr() as *mut i8;
    allocator::adopt(alloc, s);
    allocator::adopt(alloc, String_t { len, bytes })
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_addUser_ipv6(
    password: *mut String_t,
    login: *mut String_t,
    ipv6: *mut u8,
    ca: *mut Rffi_CryptoAuth2_t,
) -> c_int {
    let ip6 = if ipv6.is_null() {
        None
    } else {
        let mut ip = [0_u8; 16];
        ip.copy_from_slice(std::slice::from_raw_parts(ipv6, 16));
        Some(ip)
    };
    match (*ca)
        .0
        .add_user_ipv6(cstr(password).expect("password"), cstr(login), ip6)
    {
        Ok(_) => 0,
        Err(crypto_auth::AddUserError::Duplicate { .. }) => {
            cffi::CryptoAuth_addUser_Res::CryptoAuth_addUser_DUPLICATE as i32
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_removeUsers(
    context: *mut Rffi_CryptoAuth2_t,
    user: *mut String_t,
) -> c_int {
    (*context).0.remove_users(cstr(user)) as c_int
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getUsers(
    ca: *const Rffi_CryptoAuth2_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_StrList_t {
    let mut users = (*ca).0.get_users();
    let mut str_users = users.drain(..).map(|u| strc(alloc, u)).collect::<Vec<_>>();
    let out = RTypes_StrList_t {
        len: str_users.len(),
        items: str_users.as_mut_ptr(),
    };
    allocator::adopt(alloc, str_users);
    allocator::adopt(alloc, out)
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_new(
    allocator: *mut Allocator_t,
    privateKey: *const u8,
    random: *mut Random_t,
) -> *mut Rffi_CryptoAuth2_t {
    allocator::adopt(
        allocator,
        Rffi_CryptoAuth2_t(Arc::new(crypto_auth::CryptoAuth::new(
            if privateKey.is_null() {
                None
            } else {
                let mut bytes = [0_u8; 32];
                bytes.copy_from_slice(std::slice::from_raw_parts(privateKey, 32));
                Some(PrivateKey::from(bytes))
            },
            crate::util::events::EventBase {},
            //crate::crypto::random::Random::new_sodium().expect("libsodium init() failed"),
            crate::crypto::random::Random::wrap_legacy(random),
        ))),
    )
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_newSession(
    ca: *mut Rffi_CryptoAuth2_t,
    alloc: *mut Allocator_t,
    herPublicKey: *const u8,
    requireAuth: bool,
    name: *mut c_char,
    useNoise: bool,
) -> *mut Rffi_CryptoAuth2_Session_t {
    let (session, plaintext, ciphertext) = crypto_auth::Session::new(
        Arc::clone(&(*ca).0),
        {
            let mut bytes = [0_u8; 32];
            bytes.copy_from_slice(std::slice::from_raw_parts(herPublicKey, 32));
            PublicKey::from(bytes)
        },
        requireAuth,
        if name.is_null() {
            None
        } else {
            match std::ffi::CStr::from_ptr(name).to_str() {
                Ok(s) => Some(s.to_string()),
                Err(_) => None,
            }
        },
        useNoise,
    )
    .expect("bad public key"); //TODO Pass the error to C code somehow instead of `expect()`.
    allocator::adopt(
        alloc,
        Rffi_CryptoAuth2_Session_t {
            s: session,
            ciphertext: cif::wrap(alloc, ciphertext),
            plaintext: cif::wrap(alloc, plaintext),
        },
    )
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_setAuth(
    password: *const String_t,
    login: *const String_t,
    caSession: *mut Rffi_CryptoAuth2_Session_t,
) {
    (*caSession).s.set_auth(cstr(password), cstr(login))
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_resetIfTimeout(session: *mut Rffi_CryptoAuth2_Session_t) {
    (*session).s.reset_if_timeout()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_reset(caSession: *mut Rffi_CryptoAuth2_Session_t) {
    (*caSession).s.reset()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getState(
    session: *mut Rffi_CryptoAuth2_Session_t,
) -> RTypes_CryptoAuth_State_t {
    (*session).s.get_state()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getHerPubKey(
    session: *const Rffi_CryptoAuth2_Session_t,
    pkOut: *mut u8,
) {
    let p = (*session).s.get_her_pubkey();
    std::slice::from_raw_parts_mut(pkOut, 32).copy_from_slice(&p[..]);
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getHerIp6(
    session: *const Rffi_CryptoAuth2_Session_t,
    ipOut: *mut u8,
) {
    let p = (*session).s.get_her_ip6();
    std::slice::from_raw_parts_mut(ipOut, 16).copy_from_slice(&p[..]);
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getName(
    session: *const Rffi_CryptoAuth2_Session_t,
    alloc: *mut Allocator_t,
) -> *mut String_t {
    match (*session).s.get_name() {
        None => 0 as *mut String_t,
        Some(name) => strc(alloc, ByteString::from(name)),
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getPubKey(ca: *mut Rffi_CryptoAuth2_t, pkOut: *mut u8) {
    let p = (*ca).0.public_key.raw();
    std::slice::from_raw_parts_mut(pkOut, 32).copy_from_slice(&p[..]);
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_stats(
    session: *const Rffi_CryptoAuth2_Session_t,
    statsOut: *mut RTypes_CryptoStats_t,
) {
    let st = (*session).s.stats();
    (*statsOut) = st;
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_panic(msg: *const c_char) -> ! {
    panic!("{}", std::ffi::CStr::from_ptr(msg).to_string_lossy())
}
