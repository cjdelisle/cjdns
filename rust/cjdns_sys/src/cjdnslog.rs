use parking_lot::Mutex;

use crate::cffi;

use std::ffi::CString;

pub struct CjdnsLog {
    log: Mutex<*mut cffi::Log>,
}
unsafe impl Send for CjdnsLog {}
unsafe impl Sync for CjdnsLog {}

impl CjdnsLog {
    pub fn new(l: *mut cffi::Log) -> CjdnsLog {
        CjdnsLog { log: Mutex::new(l) }
    }
}

impl Drop for CjdnsLog {
    fn drop(&mut self) {
        panic!("cjdnslog dropped");
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
        let msg = CString::new(format!("{}", record.args())).unwrap();
        let log = self.log.lock();
        unsafe {
            cffi::Log_print0(
                *log,
                lvl,
                filebuf.as_ptr() as *const i8,
                line as i32,
                msg.as_ptr() as *const i8,
            )
        };
    }
    fn flush(&self) {}
}
