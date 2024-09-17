use std::time::Duration;

use crate::util::{now_ms,now_duration};

/// Non-monotonic nanosecond time, which has no relationship to any wall clock.
#[no_mangle]
pub extern "C" fn Rffi_hrtime() -> u64 {
    now_duration().as_nanos() as u64
}

/// Monotonic millisecond time.
#[no_mangle]
pub extern "C" fn Rffi_now_ms() -> u64 {
    now_ms()
}

#[no_mangle]
pub extern "C" fn Rffi_sleep_ms_sync(ms: u64) {
    std::thread::sleep(Duration::from_millis(ms));
}
