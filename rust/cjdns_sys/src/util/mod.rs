pub mod sockaddr;
pub mod identity;

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