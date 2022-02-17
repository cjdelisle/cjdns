use once_cell::sync::Lazy;
use std::time::{Duration, Instant, SystemTime};

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
    static BASE_INSTANT: Lazy<Instant> = Lazy::new(Instant::now);
    static INSTANT_OFFSET: Lazy<u64> = Lazy::new(|| now_unix_epoch().as_millis() as _);
    Lazy::force(&BASE_INSTANT);

    (Instant::now() - *BASE_INSTANT).as_millis() as u64 + *INSTANT_OFFSET
}
