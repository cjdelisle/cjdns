#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
use crate::cffi::CryptoAuth_State as ffist;
use crate::cffi::{self, Allocator_t, Iface_t, Message_t, String_t};
use crate::crypto::crypto_auth;
use crate::crypto::keys::{IpV6, PrivateKey, PublicKey};
use crate::external::interface::cif;
use crate::external::memory::allocator;
use crate::interface::wire::message::Message;

use std::os::raw::{c_char, c_int};
use std::sync::Arc;

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

pub struct Rffi_CryptoAuth2_t(Arc<crypto_auth::CryptoAuth>);
pub struct Rffi_CryptoAuth2_Session_t(crypto_auth::Session);

unsafe fn cu8(s: *const u8, len: usize) -> Vec<u8> {
    let x = std::slice::from_raw_parts::<u8>(s, len);
    let mut out = Vec::with_capacity(x.len());
    out.copy_from_slice(x);
    out
}

unsafe fn cstr(s: *const String_t) -> Option<Vec<u8>> {
    if s.is_null() {
        None
    } else {
        Some(cu8((*s).bytes as *const u8, (*s).len))
    }
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
        let mut ip = IpV6::default();
        ip.0.copy_from_slice(&cu8(ipv6, 16));
        Some(ip)
    };
    match (*ca)
        .0
        .add_user_ipv6(&cstr(password).unwrap(), cstr(login), ip6)
    {
        Ok(_) => 0,
        Err(crypto_auth::AddUserError::Duplicate) => {
            cffi::CryptoAuth_addUser_Res::CryptoAuth_addUser_DUPLICATE as i32
        }
    }
}

#[no_mangle]
pub unsafe fn Rffi_CryptoAuth2_removeUsers(
    context: *mut Rffi_CryptoAuth2_t,
    user: *mut String_t,
) -> c_int {
    (*context).0.remove_users(cstr(user)) as c_int
}

#[repr(C)]
pub struct Rffi_StrList_t {
    len: usize,
    items: *mut String_t,
}

// TODO(cjd): we can't return a Vec<Vec<u8>> to C because it becomes opaque
// needs fixin'
#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getUsers(
    ca: *const Rffi_CryptoAuth2_t,
    alloc: *mut Allocator_t,
) -> *mut Vec<Vec<u8>> {
    let users = (*ca).0.get_users();
    allocator::adopt(alloc, (*ca).0.get_users())
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_new(
    allocator: *mut Allocator_t,
    privateKey: *const u8,
) -> *mut Rffi_CryptoAuth2_t {
    allocator::adopt(
        allocator,
        Rffi_CryptoAuth2_t(Arc::new(crypto_auth::CryptoAuth::new(
            if privateKey.is_null() {
                None
            } else {
                Some(PrivateKey::from(&cu8(privateKey, 32)[..]))
            },
            crate::util::events::EventBase {},
            crate::crypto::random::Random {},
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
) -> *mut Rffi_CryptoAuth2_Session_t {
    allocator::adopt(
        alloc,
        Rffi_CryptoAuth2_Session_t(crypto_auth::Session::new(
            Arc::clone(&(*ca).0),
            {
                let mut x: [u8; 32] = [0; 32];
                x.copy_from_slice(std::slice::from_raw_parts(herPublicKey, 32));
                PublicKey(x)
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
        )),
    )
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_encrypt(
    session: *mut Rffi_CryptoAuth2_Session_t,
    msg: *mut Message_t,
) -> c_int {
    let mut msg = Message::from_c_message(msg);
    match (*session).0.encrypt(&mut msg) {
        Ok(_) => 0,
        Err(_) => -1,
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_decrypt(
    sess: *mut Rffi_CryptoAuth2_Session_t,
    msg: *mut Message_t,
) -> c_int {
    let mut msg = Message::from_c_message(msg);
    match (*sess).0.decrypt(&mut msg) {
        Ok(_) => 0,
        Err(e) => e as c_int,
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_setAuth(
    password: *const String_t,
    login: *const String_t,
    caSession: *mut Rffi_CryptoAuth2_Session_t,
) {
    (*caSession).0.set_auth(cstr(password), cstr(login))
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_resetIfTimeout(session: *mut Rffi_CryptoAuth2_Session_t) {
    (*session).0.reset_if_timeout()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_reset(caSession: *mut Rffi_CryptoAuth2_Session_t) {
    (*caSession).0.reset()
}

#[no_mangle]
unsafe extern "C" fn Rffi_CryptoAuth2_getState(
    session: *mut Rffi_CryptoAuth2_Session_t,
) -> cffi::CryptoAuth_State {
    match (*session).0.get_state() {
        crypto_auth::State::Init => ffist::CryptoAuth_State_INIT,
        crypto_auth::State::SentHello => ffist::CryptoAuth_State_SENT_HELLO,
        crypto_auth::State::ReceivedHello => ffist::CryptoAuth_State_RECEIVED_HELLO,
        crypto_auth::State::SentKey => ffist::CryptoAuth_State_SENT_KEY,
        crypto_auth::State::ReceivedKey => ffist::CryptoAuth_State_RECEIVED_KEY,
        crypto_auth::State::Established => ffist::CryptoAuth_State_ESTABLISHED,
    }
}
