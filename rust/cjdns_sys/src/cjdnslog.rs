//! Logger implementation that sends all messages to the native C logger.
//!
//! To use this logger, first call `install()` early in your program,
//! this mut only be done once, and then call `set_ffi_logger()` at least one time.
//!
//! Repeated calls to `set_ffi_logger()` are allowed, but if it is not called at all,
//! all log messages would be lost.

use std::ffi::CString;
use std::ptr::null_mut;
use std::os::raw::c_char;

use parking_lot::{const_mutex, Mutex};

use crate::cffi;

/// Wrapper over native C logger
pub struct CjdnsLog {
    log: Mutex<*mut cffi::Log>,
}
unsafe impl Send for CjdnsLog {}
unsafe impl Sync for CjdnsLog {}

/// The only instance of CjdnsLog, never dropped
static INSTANCE: CjdnsLog = CjdnsLog::new();

impl Drop for CjdnsLog {
    fn drop(&mut self) {
        panic!("CjdnsLog instance dropped - should never happen");
    }
}

/// Install the logger, call early in the program.
pub fn install() {
    match log::set_logger(&INSTANCE) {
        Ok(_) => (),
        Err(e) => panic!("Unable to set logger: {}", e),
    }
    log::set_max_level(env_logger::Logger::from_default_env().filter());
}

/// Set the native logger, call at least once after calling `install()`.
pub fn set_ffi_logger(l: *mut cffi::Log) {
    INSTANCE.set_ffi_logger_impl(l);
}

impl CjdnsLog {
    const fn new() -> CjdnsLog {
        CjdnsLog { log: const_mutex(null_mut()) }
    }

    fn set_ffi_logger_impl(&self, l: *mut cffi::Log) {
        let mut log = self.log.lock();
        *log = l;
    }
}

impl log::Log for CjdnsLog {
    fn enabled(&self, _metadata: &log::Metadata<'_>) -> bool {
        true
    }

    fn log(&self, record: &log::Record<'_>) {
        let lvl = match record.level() {
            log::Level::Error => cffi::Log_Level::Log_Level_ERROR,
            log::Level::Warn => cffi::Log_Level::Log_Level_WARN,
            log::Level::Info => cffi::Log_Level::Log_Level_INFO,
            log::Level::Debug => cffi::Log_Level::Log_Level_DEBUG,
            log::Level::Trace => cffi::Log_Level::Log_Level_DEBUG, // we have no TRACE level
        };
        let mut filebuf = [0_u8; 32];
        let file = record.file().unwrap_or("<unknown>");
        let file_slice = match file.rfind('/') {
            Some(i) => &file[i + 1..],
            None => &file[..],
        }
        .as_bytes();
        if file_slice.len() > 31 {
            filebuf[0..31].copy_from_slice(&file_slice[0..31]);
        } else {
            filebuf[0..file_slice.len()].copy_from_slice(file_slice);
        }
        let line = record.line().unwrap_or(0);
        let msg = format!("{}", record.args());
        let log = self.log.lock();
        if log.is_null() {
            // Suppress logs when no logger is yet configured.
            //println!("{} {}:{} {}", record.level().as_str(), file, line, msg);
            return;
        }
        let cmsg = CString::new(msg).unwrap();
        unsafe {
            cffi::Log_print0(
                *log,
                lvl,
                filebuf.as_ptr() as *const c_char,
                line as i32,
                cmsg.as_ptr() as *const c_char,
            )
        };
    }

    fn flush(&self) {}
}
