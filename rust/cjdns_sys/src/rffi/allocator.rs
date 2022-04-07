use std::sync::{Arc,Weak};
use parking_lot::Mutex;
use tokio::sync::oneshot;
use std::ffi::{c_void, CStr};
use std::os::raw::c_char;
use std::any::Any;
use crate::cffi::Allocator_t;
use async_recursion::async_recursion;
use std::cell::RefCell;

struct Mem {
    alloc_ident: Arc<String>,
    loc: Vec<u128>,
}
impl Drop for Mem {
    fn drop(&mut self) {
        println!("Dropping memory {:p} from {}", &self.loc, self.alloc_ident);
        for i in 0..self.loc.len() {
            self.loc[i] = 0xefefefefefefefefefefefefefefefef_u128;
        }
    }
}

struct AllocatorInner {
    parents: Mutex<Vec<Weak<AllocatorInner>>>,
    children: Mutex<Vec<Allocator>>,
    mem: Mutex<Vec<Mem>>,
    obj: Mutex<Vec<Box<dyn Any + Send>>>,
    on_free: Mutex<Vec<OnFreeJob>>,
    ident: RefCell<Arc<String>>,
}
unsafe impl Send for AllocatorInner {}
unsafe impl Sync for AllocatorInner {}

struct OnFreeJob {
    f: OnFreeFun,
    c: *mut c_void,
}
unsafe impl Send for OnFreeJob {}

fn as_count(size: usize) -> usize {
    let mut count = size / 16;
    if count * 16 != size {
        count += 1;
    }
    count
}

#[async_recursion]
async fn free_alloc(alloc: &Arc<AllocatorInner>) {
    let parents = alloc.parents.lock().drain(..).collect::<Vec<_>>();
    let children = alloc.children.lock().drain(..).collect::<Vec<_>>();
    let jobs = alloc.on_free.lock().drain(..).collect::<Vec<_>>();
    for p in parents {
        if let Some(p) = p.upgrade() {
            p.children.lock().retain(|c| !std::ptr::eq(&c.0.obj, &alloc.obj));
        }
    }
    for c in children {
        {
            let mut pl = c.0.parents.lock();
            pl.retain(|c| if let Some(c) = c.upgrade() { !std::ptr::eq(&c.obj, &alloc.obj) } else { false });
            if pl.len() > 0 {
                continue;
            }
        }
        free_alloc(&c.0).await;
    }
    for job in jobs {
        let (tx, rx) = oneshot::channel();
        let ofc = Arc::new(OnFreeCtx(Some(tx)));
        (job.f)(job.c, &*ofc as *const OnFreeCtx as *mut OnFreeCtx);
        rx.await.unwrap();
    }
}

struct Adoption<T>(T);
unsafe impl<T> Send for Adoption<T> {}

// ---- //

pub struct Allocator(Arc<AllocatorInner>);

pub struct OnFreeCtx(Option<oneshot::Sender<()>>);

pub type OnFreeFun = extern "C" fn(ctx: *mut c_void, complete: *mut OnFreeCtx);

macro_rules! new {
    () => {
        Allocator::new(file!().as_ptr() as *const std::os::raw::c_char, line!() as usize)
    }
}
pub(crate) use new;
macro_rules! child {
    ($a:expr) => {
        $a.child(file!().as_ptr() as *const std::os::raw::c_char, line!() as usize)
    }
}
pub(crate) use child;

impl Allocator {
    pub fn new(file: *const c_char, line: usize) -> Allocator {
        let mut a = Allocator(Arc::new(AllocatorInner{
            parents: Mutex::default(),
            children: Mutex::default(),
            mem: Mutex::default(),
            obj: Mutex::default(),
            on_free: Mutex::default(),
            ident: RefCell::default(),
        }));
        a.0.ident.replace(
            Arc::new(format!("{:?}:{}/{:p}", unsafe { CStr::from_ptr(file) }, line, &a.0.obj))
        );

        println!("New root allocator {}", a.ident());
        a
    }

    pub fn ident(&self) -> Arc<String> {
        Arc::clone(&*self.0.ident.borrow())
    }

    pub fn child(&self, file: *const c_char, line: usize) -> Allocator {
        let mut a = Allocator(Arc::new(AllocatorInner{
            parents: Mutex::new(vec![ Arc::downgrade(&self.0) ]),
            children: Mutex::default(),
            mem: Mutex::default(),
            obj: Mutex::default(),
            on_free: Mutex::default(),
            ident: RefCell::default(),
        }));
        self.0.children.lock().push(Allocator(Arc::clone(&a.0)));
        a.0.ident.replace(
            Arc::new(format!("{:?}:{}/{:p}", unsafe { CStr::from_ptr(file) }, line, &a.0.obj))
        );
        println!("New child allocator {} <- {}", a.ident(), self.ident());
        a
    }

    pub fn malloc(&self, size: usize, zero_mem: bool) -> *mut u8 {
        if size == 0 {
            return std::ptr::null_mut();
        }
        let count = as_count(size);
        let mut m = Mem{ loc: Vec::with_capacity(count), alloc_ident: self.ident() };
        if zero_mem {
            for _ in 0..count {
                m.loc.push(0);
            }
        } else {
            unsafe { m.loc.set_len(count); }
        }
        let p = &m.loc[0] as *const u128 as *const u8 as *mut u8;
        self.0.mem.lock().push(m);
        p
    }

    pub fn realloc(&self, memptr: *mut u8, new_size: usize) -> *mut u8 {
        if memptr.is_null() {
            return self.malloc(new_size, false);
        }
        for m in self.0.mem.lock().iter_mut() {
            let p = &m.loc[0] as *const u128 as *const u8 as *mut u8;
            if p == memptr {
                m.loc.resize(as_count(new_size), 0);
                return &m.loc[0] as *const u128 as *const u8 as *mut u8;
            }
        }
        panic!("pointer {:p} is not in memory allocator {:p}", memptr, self);
    }

    pub fn on_free(&self, f: OnFreeFun, c: *mut c_void) {
        self.0.on_free.lock().push(OnFreeJob{f, c});
    }

    pub fn adopt_alloc(&self, a: Allocator) {
        a.0.parents.lock().push(Arc::downgrade(&self.0));
        self.0.children.lock().push(a);
    }

    pub fn adopt<T: 'static>(&self, t: T) -> *mut T {
        let mut b: Box<dyn Any + Send> = Box::new(Adoption(t));
        let t = &mut b.downcast_mut::<Adoption<T>>().unwrap().0 as *mut T;
        self.0.obj.lock().push(b);
        t
    }

    pub fn c(&mut self) -> *mut Allocator_t {
        self as *mut Allocator as *mut Allocator_t
    }
}

impl Drop for Allocator {
    fn drop(&mut self) {
        let parent_count = {
            self.0.parents.lock().iter().filter(|p|p.strong_count() > 0).count()
        };
        let sc = Arc::strong_count(&self.0);
        //println!("Dropping allocator {} ref count {}", self.ident(), sc);
        if sc > parent_count + 1 {
            return;
        }
        assert!(sc == parent_count + 1);
        println!("Dropping allocator {}", self.ident());
        let a = Arc::clone(&self.0);
        tokio::spawn(async {
            let a = a;
            free_alloc(&a).await
        });
    }
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_onFreeComplete(c: *mut OnFreeCtx) {
    if let Some(x) = unsafe { (*c).0.take() } {
        x.send(()).unwrap();
    } else {
        panic!("onFreeComplete called twice");
    }
}

/// Create a root level allocator.
#[no_mangle]
pub extern "C" fn Rffi_allocator_newRoot(file: *const c_char, line: usize) -> *mut Allocator_t {
    let a = Box::new(Allocator::new(file, line));
    Box::into_raw(a) as *mut Allocator_t
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_free(a: *mut Allocator_t) {
    let b = unsafe { Box::from_raw(a as *mut Allocator) };
    drop(b)
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_child(a: *mut Allocator_t, file: *const c_char, line: usize) -> *mut Allocator_t {
    Box::into_raw(Box::new(rs(a).child(file, line))) as *mut Allocator_t
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_malloc(a: *mut Allocator_t, size: usize) -> *mut u8 {
    rs(a).malloc(size, false)
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_calloc(a: *mut Allocator_t, size: usize) -> *mut u8 {
    rs(a).malloc(size, true)
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_realloc(a: *mut Allocator_t, ptr: *mut u8, new_size: usize) -> *mut u8 {
    rs(a).realloc(ptr, new_size)
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_onFree(a: *mut Allocator_t, fun: OnFreeFun, ctx: *mut c_void) {
    rs(a).on_free(fun, ctx)
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_adopt(a: *mut Allocator_t, to_adopt: *mut Allocator_t) {
    rs(a).adopt_alloc(rs(to_adopt))
}

pub fn adopt<T: 'static>(a: *mut Allocator_t, t: T) -> *mut T {
    rs(a).adopt(t)
}

pub fn rs(a: *mut Allocator_t) -> Allocator {
    Allocator(Arc::clone(&unsafe { &*(a as *mut Allocator) }.0))
}