use once_cell::sync::Lazy;
use parking_lot::ReentrantMutex;

/// Global C lock, to make callbacks into C, while keeping libuv's and tokio's async Runtimes synced.
pub static GCL: Lazy<ReentrantMutex<()>> = Lazy::new(|| ReentrantMutex::new(()));

