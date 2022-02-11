//!
//! This file is used to generate Rffi.h using cbindgen.
//!

#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

use crate::bytestring::ByteString;
use crate::cffi::{self, Allocator_t, EventBase_ref, EventBase_unref, Random_t, String_t};
use crate::crypto::crypto_auth;
use crate::crypto::crypto_auth::DecryptError;
use crate::crypto::keys::{PrivateKey, PublicKey};
use crate::crypto::session;
use crate::external::interface::cif;
use crate::external::memory::allocator;
use crate::interface::wire::message::Message;
use crate::rtypes::*;
use once_cell::sync::Lazy;
use pnet::util::MacAddr;
use std::convert::TryInto;
use std::ffi::{c_void, CStr};
use std::os::raw::{c_char, c_int, c_long, c_ulong};
use std::os::unix::process::ExitStatusExt;
use std::sync::{Arc, Mutex, MutexGuard, Weak};
use std::time::{Duration, Instant, SystemTime};
use tokio::process::Command;

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
    let bytes = s.as_mut_ptr() as *mut c_char;
    allocator::adopt(alloc, s);
    allocator::adopt(alloc, String_t { len, bytes })
}

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
    let (plaintext, ciphertext) = session.ifaces().unwrap();
    &mut allocator::adopt(
        alloc,
        Rffi_CryptoAuth2_Session_t {
            r: RTypes_CryptoAuth2_Session_t {
                ciphertext: cif::wrap(alloc, ciphertext),
                plaintext: cif::wrap(alloc, plaintext),
            },
            s: session,
        },
    )
    .r as *mut _
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_CryptoAuth2_tryHandshake(
    ca: *mut RTypes_CryptoAuth2_t,
    c_msg: *mut cffi::Message_t,
    alloc: *mut Allocator_t,
    requireAuth: bool,
    ret: *mut RTypes_CryptoAuth2_TryHandshake_Ret_t,
) {
    let mut msg = Message::from_c_message(c_msg);
    match crypto_auth::try_handshake(&(*ca).0, &mut msg, requireAuth) {
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
                let child = cffi::Allocator__child(
                    alloc,
                    b"rffi_tryHandshake\0".as_ptr() as *const c_char,
                    163,
                );
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
    let mut alloc = allocator::Allocator::wrap(alloc);
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

fn str_to_c(s: &str, alloc: *mut Allocator_t) -> *const c_char {
    let c_str = std::ffi::CString::new(s).unwrap();
    let adopted = allocator::adopt(alloc, c_str);
    return (*adopted).as_ptr();
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

/// Convert IPv4 and IPv6 addresses from binary to text form.
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

/// Convert IPv4 and IPv6 addresses from text to binary form.
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
pub extern "C" fn Rffi_hrtime() -> u64 {
    now_unix_epoch().as_nanos() as u64
}

fn now_unix_epoch() -> Duration {
    SystemTime::now()
        .duration_since(SystemTime::UNIX_EPOCH)
        .unwrap()
}

/// Monotonic millisecond time.
#[no_mangle]
pub extern "C" fn Rffi_now_ms() -> u64 {
    static BASE_INSTANT: Lazy<Instant> = Lazy::new(|| Instant::now());
    static INSTANT_OFFSET: Lazy<u64> = Lazy::new(|| now_unix_epoch().as_millis() as _);
    Lazy::force(&BASE_INSTANT);

    (Instant::now() - *BASE_INSTANT).as_millis() as u64 + *INSTANT_OFFSET
}

#[repr(C)]
#[derive(Debug)]
pub struct Rffi_Address {
    pub octets: [u8; 16],
    pub netmask: [u8; 16],
    pub is_ipv6: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct Rffi_NetworkInterface {
    pub name: *const c_char,
    pub phys_addr: [u8; 6],
    pub is_internal: bool,
    pub address: Rffi_Address,
}

fn to_array<const N: usize>(value: impl AsRef<[u8]>) -> [u8; N] {
    let mut value = value.as_ref().to_owned();
    value.resize(N, 0);
    value.try_into().unwrap()
}

/// Get a list of available network interfaces for the current machine.
#[no_mangle]
pub extern "C" fn Rffi_interface_addresses(
    out: *mut *const Rffi_NetworkInterface,
    alloc: *mut Allocator_t,
) -> i32 {
    let getn = |ip| match ip {
        std::net::IpAddr::V4(ip) => ip.octets().to_vec(),
        std::net::IpAddr::V6(ip) => ip.octets().to_vec(),
    };
    let mac = |mac: Option<MacAddr>| to_array(mac.map_or(vec![], |ma| ma.octets().to_vec()));

    let nis = pnet::datalink::interfaces()
        .iter()
        .map(|ni| (ni, str_to_c(&ni.name, alloc), mac(ni.mac)))
        .flat_map(|(ni, name, phys_addr)| ni.ips.iter().map(move |ip| (ni, name, phys_addr, ip)))
        .map(|(ni, name, phys_addr, ipnet)| Rffi_NetworkInterface {
            name,
            phys_addr,
            is_internal: ni.is_loopback(),
            address: Rffi_Address {
                octets: to_array(getn(ipnet.ip())),
                netmask: to_array(getn(ipnet.mask())),
                is_ipv6: ipnet.is_ipv6(),
            },
        })
        .collect::<Vec<_>>();
    let count = nis.len();
    unsafe {
        *out = (*allocator::adopt(alloc, nis)).as_mut_ptr();
    }
    count as _
}

/// Get the full filesystem path of the current running executable.
#[no_mangle]
pub extern "C" fn Rffi_exepath(out: *mut *const c_char, alloc: *mut Allocator_t) -> i32 {
    let path = match std::env::current_exe() {
        Ok(p) => p,
        Err(_) => return -1,
    };
    unsafe {
        *out = str_to_c(path.to_string_lossy().as_ref(), alloc);
    }
    0
}

/// Spawn a new child process, and monitors its result.
#[no_mangle]
pub unsafe extern "C" fn Rffi_spawn(
    file: *const c_char,
    args: *const *const c_char,
    num_args: c_int,
    _alloc: *mut Allocator_t, // perhaps create some Droppable and adopt it here, to kill the process.
    cb: Option<unsafe extern "C" fn(c_long, c_int)>,
) -> i32 {
    let file = match CStr::from_ptr(file).to_str() {
        Ok(f) => f,
        Err(_) => return -1,
    };
    let args = match std::slice::from_raw_parts(args, num_args as _)
        .iter()
        .map(|a| CStr::from_ptr(*a).to_str())
        .collect::<Result<Vec<_>, _>>()
    {
        Ok(args) => args,
        Err(_) => return -2,
    };
    let child_status = Command::new(file).args(&args).status();

    EventBase_ref();
    tokio::spawn(async move {
        match (child_status.await, cb) {
            (Ok(status), Some(callback)) => {
                let _guard = GCL.lock().unwrap();
                callback(
                    status.code().unwrap_or(-127) as _,
                    status.signal().unwrap_or(-127),
                )
            }
            (Ok(_), None) => {}
            (Err(err), _) => eprintln!("  error spawning child '{}': {:?}", file, err),
        }
        EventBase_unref()
    });
    0
}

/// Used only internally, to send `*mut c_void` into a tokio task.
struct TimerCallback {
    f: unsafe extern "C" fn(*mut c_void),
    ctx: *mut c_void,
}

// SAFETY: this only holds if the C code is thread-safe, or the tokio Runtime uses only a single thread.
unsafe impl Send for TimerCallback {}

/// Commands to control a Timer task.
enum TimerCommand {
    Reset(c_ulong),
    Cancel,
}

/// The handle returned to C, used to talk to the timer task.
pub struct Rffi_TimerTx(tokio::sync::mpsc::UnboundedSender<TimerCommand>);

/// Keep a loose track of all timers created, for clearAll.
static TIMER_COLLECTION: Lazy<Mutex<Vec<Weak<Rffi_TimerTx>>>> = Lazy::new(|| Mutex::new(vec![]));

/// Spawn a timer task for a timeout or interval, that calls some callback whenever it triggers.
#[no_mangle]
pub extern "C" fn Rffi_setTimeout(
    out_timer_tx: *mut *const Rffi_TimerTx,
    cb: unsafe extern "C" fn(*mut c_void),
    cb_context: *mut c_void,
    timeout_millis: c_ulong,
    repeat: bool,
    alloc: *mut Allocator_t,
) {
    let cb = TimerCallback {
        f: cb,
        ctx: cb_context,
    };

    // it must be unbounded, since its Sender is sync, and can be used directly by the controller methods.
    let (tx, mut rx) = tokio::sync::mpsc::unbounded_channel();
    let rtx = Arc::new(Rffi_TimerTx(tx));
    TIMER_COLLECTION.lock().unwrap().push(Arc::downgrade(&rtx));
    unsafe {
        // Arc::as_ptr => The counts are not affected in any way and the Arc is not consumed.
        // The pointer is valid for as long as there are strong counts in the Arc.
        *out_timer_tx = Arc::as_ptr(allocator::adopt(alloc, rtx));
    }

    unsafe { EventBase_ref() }
    tokio::spawn(async move {
        let mut timeout_millis = timeout_millis;
        loop {
            tokio::select! {
                _ = tokio::time::sleep(Duration::from_millis(timeout_millis)) => {
                    let _guard = GCL.lock().unwrap();
                    unsafe { (cb.f)(cb.ctx); }
                    if !repeat {
                        break;
                    }
                }
                msg = rx.recv() => {
                    match msg {
                        Some(TimerCommand::Reset(new_timeout)) => {
                            println!("Received reset command: {} to {}", timeout_millis, new_timeout);
                            timeout_millis = new_timeout;
                        },
                        Some(TimerCommand::Cancel) => {
                            println!("Received cancel command");
                            break
                        },
                        None => {
                            println!("Allocator freed, stopping timer task");
                            break
                        },
                    }
                }
            }
        }
        println!("  timer task stopped");
        unsafe { EventBase_unref() }
    });
}

impl Rffi_TimerTx {
    fn send(&self, msg: TimerCommand) -> c_int {
        self.0.send(msg).map_or(-1, |_| 0)
    }
}

/// Reset a timer task to change its timing.
#[no_mangle]
pub extern "C" fn Rffi_resetTimeout(
    timer_tx: *const Rffi_TimerTx,
    timeout_millis: c_ulong,
) -> c_int {
    let timer_tx = unsafe { &*timer_tx };
    timer_tx.send(TimerCommand::Reset(timeout_millis))
}

/// Cancel a timer task.
#[no_mangle]
pub extern "C" fn Rffi_clearTimeout(timer_tx: *const Rffi_TimerTx) -> c_int {
    let timer_tx = unsafe { &*timer_tx };
    timer_tx.send(TimerCommand::Cancel)
}

/// Cancel all timer tasks.
#[no_mangle]
pub extern "C" fn Rffi_clearAllTimeouts() -> c_int {
    TIMER_COLLECTION
        .lock()
        .unwrap()
        .drain(..)
        .filter_map(|w| w.upgrade())
        .map(|timer_tx| timer_tx.send(TimerCommand::Cancel))
        .min()
        .unwrap_or_default()
}

/// Global C lock, to make callbacks into C, while keeping libuv's and tokio's async Runtimes synced.
static GCL: Lazy<Mutex<()>> = Lazy::new(|| Mutex::new(()));

pub struct Rffi_Glock_guard(MutexGuard<'static, ()>);

/// Helper function to lock the Global C Lock, used only within libuv's core runtime (unix and windows).
#[no_mangle]
pub extern "C" fn Rffi_glock() -> *mut Rffi_Glock_guard {
    let guard = GCL.lock().unwrap();
    Box::into_raw(Box::new(Rffi_Glock_guard(guard)))
}

/// Helper function to unlock the Global C Lock, as noted above.
#[no_mangle]
pub extern "C" fn Rffi_gunlock(guard: *mut Rffi_Glock_guard) {
    let _guard = unsafe { Box::from_raw(guard) };
    // let the guard go out of scope, be dropped, and unlock the Mutex.
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::external::memory::allocator::Allocator;

    #[test]
    fn test_interface_addresses() {
        let alloc = Allocator::new(10000000);

        let out = unsafe {
            let mut x: *const Rffi_NetworkInterface = std::ptr::null();
            let xp = &mut x as *mut *const Rffi_NetworkInterface;
            let count = Rffi_interface_addresses(xp, alloc.native);
            std::slice::from_raw_parts(x, count as _)
        };

        let ifs = pnet::datalink::interfaces();
        let count = ifs.iter().map(|ni| ni.ips.len()).sum::<usize>();
        assert_eq!(count, out.len());
    }

    #[test]
    fn test_exepath() -> anyhow::Result<()> {
        let alloc = Allocator::new(10000000);

        let out = unsafe {
            let mut x: *const c_char = std::ptr::null();
            let xp = &mut x as *mut *const c_char;
            let err = Rffi_exepath(xp, alloc.native);
            assert_eq!(err, 0);
            CStr::from_ptr(x).to_str()
        }?;

        let path = std::env::current_exe()?;
        assert_eq!(out, &path.to_string_lossy());
        Ok(())
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_interval() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let mut timer = std::ptr::null();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const std::sync::mpsc::SyncSender<_> as _,
            1,
            true,
            alloc.native,
        );

        // ensures the callback is being called repeatedly.
        for _ in 0..5 {
            rx.recv().unwrap();
        }

        let err = Rffi_resetTimeout(timer, 2);
        assert_eq!(err, 0);

        // ensures the callback is still being called.
        // we won't really test its accuracy here, since it's impossible to do it efficiently.
        for _ in 0..5 {
            rx.recv().unwrap();
        }

        let err = Rffi_clearTimeout(timer);
        assert_eq!(err, 0);

        // ensures the callback is not called anymore.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_timeout() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let mut timer = std::ptr::null();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const std::sync::mpsc::SyncSender<_> as _,
            1,
            false,
            alloc.native,
        );

        // ensures the callback is called only once.
        rx.recv().unwrap();
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();

        let err = Rffi_resetTimeout(timer, 200);
        assert!(err < 0);

        let err = Rffi_clearTimeout(timer);
        assert!(err < 0);
    }

    #[tokio::test(flavor = "multi_thread", worker_threads = 1)]
    async fn test_free_timer() {
        let alloc = Allocator::new(10000000);

        let (tx, rx) = std::sync::mpsc::sync_channel::<u8>(1);
        unsafe extern "C" fn callback(ctx: *mut c_void) {
            println!("in callback!");
            let tx = &*(ctx as *const std::sync::mpsc::SyncSender<u8>);
            tx.send(1).unwrap();
        }

        let mut timer = std::ptr::null();
        Rffi_setTimeout(
            &mut timer as _,
            callback,
            &tx as *const std::sync::mpsc::SyncSender<_> as _,
            10,
            false,
            alloc.native,
        );

        drop(alloc);

        // ensures the callback is never called.
        rx.recv_timeout(Duration::from_millis(10)).unwrap_err();
    }
}
