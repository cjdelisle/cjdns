use std::sync::Arc;

use libc::{c_char, c_int, c_uchar, c_ulonglong};
use sodiumoxide::crypto::hash::sha512;

use super::allocator::file_line;
use super::{cstr, strc};
use crate::bytestring::ByteString;
use crate::cffi::{self, Allocator_t, Random_t, String_t};
use crate::crypto::crypto_auth;
use crate::crypto::crypto_auth::DecryptError;
use crate::crypto::keys::{PrivateKey, PublicKey};
use crate::crypto::session;
use crate::external::interface::cif;
use crate::rffi::allocator;
use crate::interface::wire::message::Message;
use crate::rtypes::*;

#[repr(C)]
pub struct Rffi_CryptoAuth2_Session_t {
    r: RTypes_CryptoAuth2_Session_t,
    s: Arc<dyn session::SessionTrait>,
}

pub struct RTypes_CryptoAuth2_t(pub Arc<crypto_auth::CryptoAuth>);

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_addUser_ipv6(
    password: *mut String_t,
    login: *mut String_t,
    ipv6: *mut u8,
    ca: *mut RTypes_CryptoAuth2_t,
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
    context: *mut RTypes_CryptoAuth2_t,
    user: *mut String_t,
) -> c_int {
    (*context).0.remove_users(cstr(user)) as c_int
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getUsers(
    ca: *const RTypes_CryptoAuth2_t,
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
) -> *mut RTypes_CryptoAuth2_t {
    allocator::adopt(
        allocator,
        RTypes_CryptoAuth2_t(Arc::new(crypto_auth::CryptoAuth::new(
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

fn wrap_session(
    session: Arc<dyn crate::crypto::session::SessionTrait>,
    alloc: *mut Allocator_t,
) -> *mut RTypes_CryptoAuth2_Session_t {
    let (mut plaintext, mut ciphertext) = session.ifaces().unwrap();
    let out = allocator::adopt(
        alloc,
        Rffi_CryptoAuth2_Session_t {
            r: RTypes_CryptoAuth2_Session_t {
                ciphertext: cif::wrap(alloc, &mut ciphertext),
                plaintext: cif::wrap(alloc, &mut plaintext),
            },
            s: session,
        },
    );
    unsafe { &mut (&mut *out).r as *mut _ }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_tryHandshake(
    ca: *mut RTypes_CryptoAuth2_t,
    c_msg: *mut cffi::Message_t,
    alloc: *mut Allocator_t,
    requireAuth: bool,
    ret: *mut RTypes_CryptoAuth2_TryHandshake_Ret_t,
) {
    let msg = Message::from_c_message(c_msg);
    match crypto_auth::try_handshake(&(*ca).0, msg, requireAuth) {
        Err(e) => {
            let ee = match e.downcast_ref::<DecryptError>() {
                Some(ee) => match ee {
                    DecryptError::DecryptErr(ee) => ee,
                    DecryptError::Internal(_) => &crate::crypto::crypto_auth::DecryptErr::Internal,
                },
                None => &crate::crypto::crypto_auth::DecryptErr::Internal,
            }
            .clone() as u32;
            (*ret).err = ee;
            (*ret).code = RTypes_CryptoAuth2_TryHandshake_Code_t::Error;
        }
        Ok((code, sess)) => {
            (*ret).code = code;
            if let Some(sess) = sess {
                let child = allocator::rs(alloc).child(file_line!());
                (*ret).alloc = child;
                (*ret).sess = wrap_session(sess, child)
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_newSession(
    ca: *mut RTypes_CryptoAuth2_t,
    alloc: *mut Allocator_t,
    herPublicKey: *const u8,
    requireAuth: bool,
    name: *const c_char,
    useNoise: bool,
) -> *mut RTypes_CryptoAuth2_Session_t {
    let session = crypto_auth::new_session(
        &(*ca).0,
        if herPublicKey.is_null() {
            panic!("public key is null");
        } else {
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
    wrap_session(session, alloc)
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_noiseTick(
    sess: *mut RTypes_CryptoAuth2_Session_t,
    alloc: *mut Allocator_t,
) -> *mut cffi::Message_t {
    let mut alloc = allocator::rs(alloc);
    match ffi_sess_mut(sess).s.tick(&mut alloc) {
        Err(e) => {
            log::warn!("Error in session::tick() {}", e);
            std::ptr::null_mut()
        }
        Ok(Some(m)) => m.as_c_message(),
        Ok(None) => std::ptr::null_mut(),
    }
}

unsafe fn ffi_sess_mut(
    s: *mut RTypes_CryptoAuth2_Session_t,
) -> &'static mut Rffi_CryptoAuth2_Session_t {
    &mut *(s as *mut Rffi_CryptoAuth2_Session_t)
}
unsafe fn ffi_sess(s: *const RTypes_CryptoAuth2_Session_t) -> &'static Rffi_CryptoAuth2_Session_t {
    &*(s as *const Rffi_CryptoAuth2_Session_t)
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_setAuth(
    password: *const String_t,
    login: *const String_t,
    caSession: *mut RTypes_CryptoAuth2_Session_t,
) {
    ffi_sess_mut(caSession)
        .s
        .set_auth(cstr(password), cstr(login))
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_resetIfTimeout(
    session: *mut RTypes_CryptoAuth2_Session_t,
) {
    ffi_sess_mut(session).s.reset_if_timeout()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_reset(caSession: *mut RTypes_CryptoAuth2_Session_t) {
    ffi_sess_mut(caSession).s.reset()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getState(
    session: *mut RTypes_CryptoAuth2_Session_t,
) -> RTypes_CryptoAuth_State_t {
    ffi_sess_mut(session).s.get_state()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getHerPubKey(
    session: *const RTypes_CryptoAuth2_Session_t,
    pkOut: *mut u8,
) {
    let p = ffi_sess(session).s.get_her_pubkey();
    std::slice::from_raw_parts_mut(pkOut, 32).copy_from_slice(&p[..]);
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getHerIp6(
    session: *const RTypes_CryptoAuth2_Session_t,
    ipOut: *mut u8,
) {
    let p = ffi_sess(session).s.get_her_ip6();
    std::slice::from_raw_parts_mut(ipOut, 16).copy_from_slice(&p[..]);
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getName(
    session: *const RTypes_CryptoAuth2_Session_t,
    alloc: *mut Allocator_t,
) -> *mut String_t {
    match ffi_sess(session).s.get_name() {
        None => std::ptr::null_mut::<String_t>(),
        Some(name) => strc(alloc, ByteString::from(name)),
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_getPubKey(
    ca: *const RTypes_CryptoAuth2_t,
    pkOut: *mut u8,
) {
    let p = (*ca).0.public_key.raw();
    std::slice::from_raw_parts_mut(pkOut, 32).copy_from_slice(&p[..]);
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_stats(
    session: *const RTypes_CryptoAuth2_Session_t,
    statsOut: *mut RTypes_CryptoStats_t,
) {
    let st = ffi_sess(session).s.stats();
    (*statsOut) = st;
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_cjdnsVer(
    session: *const RTypes_CryptoAuth2_Session_t,
) -> u32 {
    ffi_sess(session).s.cjdns_ver()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_crypto_hash_sha512(
    out: *mut c_uchar, // Output buffer (hash result)
    input: *const c_uchar, // Input buffer (data to hash)
    inlen: c_ulonglong // Length of input data
) -> c_int {
    let input_slice = std::slice::from_raw_parts(input, inlen as usize);

    // Perform the SHA-512 hash
    let hash = sha512::hash(input_slice);

    // Copy the result to the output buffer
    std::ptr::copy_nonoverlapping(hash.0.as_ptr(), out, hash.0.len());

    0 // Success
}
