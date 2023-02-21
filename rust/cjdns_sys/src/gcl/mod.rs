use once_cell::sync::Lazy;
use parking_lot::{ReentrantMutex,ReentrantMutexGuard};
use std::ops::Deref;

/// Global C lock, to make callbacks into C, while keeping libuv's and tokio's async Runtimes synced.
pub static GCL: Lazy<ReentrantMutex<()>> = Lazy::new(|| ReentrantMutex::new(()));

pub struct Protected<T: Copy> {
    t: T
}
unsafe impl<T: Copy> Send for Protected<T> {}
unsafe impl<T: Copy> Sync for Protected<T> {}

pub struct ProtectedMutexGuard<T: Copy> {
    t: T,
    lock: ReentrantMutexGuard<'static, ()>,
}
impl<T: Copy> Deref for ProtectedMutexGuard<T> {
    type Target = T;
    fn deref(&self) -> &T {
        &self.t
    }
}

impl<T: Copy> Protected<T> {
    pub fn new(t: T) -> Self {
        Self{t}
    }
    pub fn lock(&self) -> ProtectedMutexGuard<T> {
        ProtectedMutexGuard{
            t: self.t,
            lock: GCL.lock(),
        }
    }
}