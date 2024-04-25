use super::{Rffi_EventLoop, GCL};
use crate::cffi::Allocator_t;
use crate::rffi::str_to_c;
use std::ffi::CStr;
use std::os::raw::{c_char, c_int};
use std::os::unix::process::ExitStatusExt;
use tokio::process::Command;

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
    cb: Option<unsafe extern "C" fn(i64, c_int)>,
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

    tokio::task::spawn(async move {
        match (child_status.await, cb) {
            (Ok(status), Some(callback)) => {
                let _guard = GCL.lock();
                callback(
                    status.code().unwrap_or(-127) as _,
                    status.signal().unwrap_or(-127),
                )
            }
            (Ok(_), None) => {}
            (Err(err), _) => eprintln!("  error spawning child '{}': {:?}", file, err),
        }
    });
    0
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::rffi::allocator;

    #[test]
    fn test_exepath() -> anyhow::Result<()> {
        let mut alloc = allocator::new!();

        let out = unsafe {
            let mut x: *const c_char = std::ptr::null();
            let xp = &mut x as *mut *const c_char;
            let err = Rffi_exepath(xp, alloc.c());
            assert_eq!(err, 0);
            CStr::from_ptr(x).to_str()
        }?;

        let path = std::env::current_exe()?;
        assert_eq!(out, &path.to_string_lossy());
        Ok(())
    }
}
