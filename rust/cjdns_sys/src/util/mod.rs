use once_cell::sync::Lazy;
use std::time::{Duration, Instant, SystemTime};

pub mod sockaddr;
pub mod identity;
pub mod async_callable;
pub mod callable;

pub mod events {
    use std::time::{SystemTime, UNIX_EPOCH};

    pub struct EventBase;

    impl EventBase {
        pub fn current_time_seconds(&self) -> u32 {
            SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .expect("current time before Epoch")
                .as_secs() as u32
        }
    }
}

pub fn now_duration() -> Duration {
    SystemTime::now()
        .duration_since(SystemTime::UNIX_EPOCH)
        .unwrap()
}

pub fn now_ms() -> u64 {
    static BASE_INSTANT: Lazy<Instant> = Lazy::new(Instant::now);
    static INSTANT_OFFSET: Lazy<u64> = Lazy::new(|| now_duration().as_millis() as _);
    Lazy::force(&BASE_INSTANT);

    (Instant::now() - *BASE_INSTANT).as_millis() as u64 + *INSTANT_OFFSET
}