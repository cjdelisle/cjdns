use std::sync::{Arc,Weak};
use parking_lot::Mutex;
use tokio::sync::oneshot;
use std::ffi::{c_void, CStr};
use std::os::raw::c_char;
use std::any::Any;
use crate::cffi::Allocator_t;
use std::cell::{RefCell,Ref};
use crate::gcl::GCL;

struct Mem {
    loc: Vec<u128>,
}
impl Mem {
    fn destroy(&mut self, alloc_ident: &str) {
        log::trace!("Dropping memory {:p} from {}", &self.loc[0], alloc_ident);
        for i in 0..self.loc.len() {
            self.loc[i] = 0xefefefefefefefefefefefefefefefef_u128;
        }
    }
}

struct AllocatorMut {
    parents: Vec<Weak<AllocatorInner>>,
    children: Vec<Arc<AllocatorInner>>,
    mem: Vec<Mem>,
    obj: Vec<Box<dyn Any + Send>>,
    on_free: Vec<OnFreeJob>,
    is_freeing: bool,
}

struct AllocatorInner {
    m: Mutex<AllocatorMut>,
    ident: RefCell<String>,
    mebox: RefCell<*mut Allocator>,
}
unsafe impl Send for AllocatorInner {}
unsafe impl Sync for AllocatorInner {}

struct OnFreeJob {
    f: OnFreeFun,
    c: *mut c_void,
    file_line: FileLine,
}
unsafe impl Send for OnFreeJob {}

fn as_count(size: usize) -> usize {
    let mut count = size / 16;
    if count * 16 != size {
        count += 1;
    }
    count
}

// Does not disconnect alloc from parent (!)
fn get_to_free(
    parent: Option<&Arc<AllocatorInner>>,
    alloc: &Arc<AllocatorInner>,
    depth: i32,
    v: &mut Vec<(Arc<AllocatorInner>, i32)>,
) {
    let children = {
        let mut m = alloc.m.lock();
        if let Some(parent) = parent {
            m.parents.retain(|p| p.upgrade().map(|p| p.ident != parent.ident).unwrap_or(false));
            if !m.parents.is_empty() {
                println!("Continuing from child {} of {} because it has {} other parent(s):",
                    alloc.ident.borrow(), parent.ident.borrow(), m.parents.len());
                for p in m.parents.iter().filter_map(|p|p.upgrade()) {
                    println!("  {}", p.ident.borrow());
                }
                return;
            }
        }
        assert!(!m.is_freeing);
        m.is_freeing = true;
        std::mem::take(&mut m.children)
    };
    for c in children {
        get_to_free(Some(alloc), &c, depth + 1, v);
    }
    v.push((Arc::clone(alloc), depth));
}

async fn free_allocs(mut allocs: Vec<(Arc<AllocatorInner>, i32)>) {
    allocs.sort_by(|a,b|b.1.cmp(&a.1));
    for (alloc, depth) in allocs.iter() {
        let jobs = {
            let mut m = alloc.m.lock();
            assert!(m.is_freeing);
            m.on_free.drain(..).collect::<Vec<_>>()
        };
        let mut i = 0;
        let jl = jobs.len();
        for job in jobs {
            log::trace!("Freeing job {} {}/{} depth {}",
                job.file_line.print(), i, jl, depth);
            i += 1;
            let (tx, rx) = oneshot::channel();
            let ofc = Arc::new(OnFreeCtx(Some(tx)));
            {
                let _guard = GCL.lock();
                (job.f)(job.c, &*ofc as *const OnFreeCtx as *mut OnFreeCtx);
            }
            rx.await.unwrap();
        }
    }
    for (alloc, _) in allocs {
        log::trace!("Freeing {} 2", alloc.ident.borrow());
        let mems = alloc.m.lock().mem.drain(..).collect::<Vec<_>>();
        let ident = alloc.ident.borrow();
        for mut mem in mems {
            mem.destroy(&ident[..]);
        }
        // Drop the allocator box
        unsafe { Box::from_raw(*alloc.mebox.borrow()) };
    }
}

struct Adoption<T>(T);
unsafe impl<T> Send for Adoption<T> {}

// ---- //

pub struct Allocator {
    inner: Arc<AllocatorInner>,
    magic: u32,
    free_on_drop: bool,
}

const MAGIC: u32 = 0xdeaffeed;

pub struct OnFreeCtx(Option<oneshot::Sender<()>>);

pub type OnFreeFun = extern "C" fn(ctx: *mut c_void, complete: *mut OnFreeCtx);

pub struct FileLine{
    pub file_s: Option<&'static str>,
    pub file_c: Option<*const c_char>,
    pub line: usize,
}

impl FileLine {
    fn print(&self) -> String {
        if let Some(s) = self.file_s {
            format!("{}:{}", s, self.line)
        } else if let Some(file) = self.file_c {
            unsafe { format!("{}:{}", CStr::from_ptr(file).to_string_lossy(), self.line) }
        } else {
            format!("<unknown>:{}", self.line)
        }
    }
}

macro_rules! file_line {
    () => {
        $crate::rffi::allocator::FileLine{
            file_s: Some(file!()),
            file_c: None,
            line: line!() as usize,
        }
    }
}
pub(crate) use file_line;
macro_rules! new {
    () => {
        $crate::rffi::allocator::Allocator::owned(
            $crate::rffi::allocator::new_c!()
        )
    }
}
pub(crate) use new;
macro_rules! new_c {
    () => {
        $crate::rffi::allocator::Allocator::new(
            $crate::rffi::allocator::file_line!()
        )
    }
}
pub(crate) use new_c;
macro_rules! child {
    ($a:expr) => {
        $crate::rffi::allocator::Allocator::owned(
            $a.child($crate::rffi::allocator::file_line!())
        )
    }
}
pub(crate) use child;

impl Allocator {
    fn new_int(file_line: FileLine, parents: Vec<Weak<AllocatorInner>>) -> *mut Allocator_t {
        let parent = if let Some(parent) = parents.get(0).map(|p|p.upgrade()).flatten() {
            parent.ident.borrow().to_owned()
        } else {
            "<root>".to_owned()
        };
        let a = Box::new(Allocator{
            inner: Arc::new(AllocatorInner{
                m: Mutex::new(AllocatorMut{
                    parents: parents,
                    children: Vec::new(),
                    mem: Vec::new(),
                    obj: Vec::new(),
                    on_free: Vec::new(),
                    is_freeing: false,
                }),
                ident: RefCell::default(),
                mebox: RefCell::new(std::ptr::null_mut()),
            }),
            magic: MAGIC,
            free_on_drop: false,
        });
        // This "as" does not cast, but it fails if we're wrong
        let a = Box::into_raw(a);
        unsafe {
            (*a).inner.ident.replace(format!("{}/{:p}", file_line.print(), a));
            (*a).inner.mebox.replace(a);
            log::trace!("New allocator {} <- {}", (*a).ident(), parent);
        }
        a as *mut Allocator_t
    }

    pub fn owned(a: *mut Allocator_t) -> Allocator {
        let mut a = rs(a);
        a.free_on_drop = true;
        a
    }

    pub fn new(file_line: FileLine) -> *mut Allocator_t {
        Self::new_int(file_line, Vec::new())
    }

    pub fn ident(&self) -> Ref<String> {
        self.inner.ident.borrow()
    }

    pub fn child(&self, file_line: FileLine) -> *mut Allocator_t {
        let out = Self::new_int(file_line, vec![ Arc::downgrade(&self.inner) ]);
        let a = Arc::clone(&rs(out).inner);
        let mut m = self.inner.m.lock();
        assert!(!m.is_freeing, "Cannot create child while freeing");
        m.children.push(a);
        out
    }

    pub fn malloc(&self, size: usize, zero_mem: bool) -> *mut u8 {
        if size == 0 {
            return std::ptr::null_mut();
        }
        let count = as_count(size);
        let mut mem = Mem{ loc: Vec::with_capacity(count) };
        if zero_mem {
            for _ in 0..count {
                mem.loc.push(0);
            }
        } else {
            unsafe { mem.loc.set_len(count); }
        }
        let p = &mem.loc[0] as *const u128 as *const u8 as *mut u8;
        let mut m = self.inner.m.lock();
        assert!(!m.is_freeing, "Cannot allocate while freeing");
        m.mem.push(mem);
        p
    }

    pub fn realloc(&self, memptr: *mut u8, new_size: usize) -> *mut u8 {
        if memptr.is_null() {
            return self.malloc(new_size, false);
        }
        let mut m = self.inner.m.lock();
        assert!(!m.is_freeing, "Cannot allocate while freeing");
        for mem in m.mem.iter_mut() {
            let p = &mem.loc[0] as *const u128 as *const u8 as *mut u8;
            if p == memptr {
                mem.loc.resize(as_count(new_size), 0);
                return &mem.loc[0] as *const u128 as *const u8 as *mut u8;
            }
        }
        panic!("pointer {:p} is not in memory allocator {:p}", memptr, self);
    }

    pub fn on_free(&self, f: OnFreeFun, c: *mut c_void, file_line: FileLine) {
        let mut m = self.inner.m.lock();
        assert!(!m.is_freeing, "Cannot on_free() while freeing");
        m.on_free.push(OnFreeJob{f, c, file_line});
    }

    pub fn adopt_alloc(&self, a: Allocator) {
        if self.inner.ident == a.inner.ident {
            return;
        }
        // Danger: possibility of deadlock if this is in the opposite order elsewhere!
        let mut m = self.inner.m.lock();
        let mut am = a.inner.m.lock();
        assert!(!m.is_freeing, "Cannot adopt alloc while freeing");
        assert!(!am.is_freeing, "Cannot be adopted while freeing");
        if m.children.iter().find(|c|c.ident == a.inner.ident).is_some() {
            return;
        } else if a.inner.ident == self.inner.ident {
            return;
        }
        m.children.push(Arc::clone(&a.inner));
        am.parents.push(Arc::downgrade(&self.inner));
    }

    pub fn adopt<T: 'static>(&self, t: T) -> *mut T {
        let mut b: Box<dyn Any + Send> = Box::new(Adoption(t));
        let t = &mut b.downcast_mut::<Adoption<T>>().unwrap().0 as *mut T;
        let mut m = self.inner.m.lock();
        assert!(!m.is_freeing, "Cannot adopt obj while freeing");
        m.obj.push(b);
        t
    }

    pub fn c(&mut self) -> *mut Allocator_t {
        *self.inner.mebox.borrow() as _
    }

    pub fn free(&mut self) {
        let parent = {
            let mut m = self.inner.m.lock();
            if m.is_freeing {
                println!("Alloc {} is already freeing", self.ident());
                return;
            }
            m.parents.retain(|p|p.strong_count() > 0);
            m.parents.iter().filter_map(|p|p.upgrade()).next()
        };
        // Disconnect this alloc from it's parent because get_to_free does not do that
        if let Some(p) = &parent {
            p.m.lock().children.retain(|c| c.ident != self.inner.ident);
        }
        let mut v = Vec::new();
        get_to_free(parent.as_ref(), &self.inner, 0, &mut v);
        tokio::spawn(async {
            free_allocs(v).await
        });
    }
}

impl Drop for Allocator {
    fn drop(&mut self) {
        if self.free_on_drop {
            self.free();
        }
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
    Allocator::new(FileLine{ file_s: None, file_c: Some(file), line })
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_free(a: *mut Allocator_t) {
    rs(a).free()
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_isFreeing(a: *mut Allocator_t) -> std::os::raw::c_int {
    if rs(a).inner.m.lock().is_freeing {
        1
    } else {
        0
    }
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_child(a: *mut Allocator_t, file: *const c_char, line: usize) -> *mut Allocator_t {
    rs(a).child(FileLine{ file_s: None, file_c: Some(file), line })
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
pub extern "C" fn Rffi_allocator_onFree(
    a: *mut Allocator_t,
    fun: OnFreeFun,
    ctx: *mut c_void,
    file: *const c_char,
    line: usize,
) {
    rs(a).on_free(fun, ctx, FileLine{ file_s: None, file_c: Some(file), line })
}

#[no_mangle]
pub extern "C" fn Rffi_allocator_adopt(a: *mut Allocator_t, to_adopt: *mut Allocator_t) {
    rs(a).adopt_alloc(rs(to_adopt))
}

pub fn adopt<T: 'static>(a: *mut Allocator_t, t: T) -> *mut T {
    rs(a).adopt(t)
}

pub fn rs(a: *mut Allocator_t) -> Allocator {
    let b = unsafe { Box::from_raw(a as *mut Allocator) };
    assert!(b.magic == MAGIC, "Wrong allocator magic {:#08x} != {:#08x}", b.magic, MAGIC);
    let out = Allocator {
        inner: Arc::clone(&b.inner),
        magic: 0,
        free_on_drop: false,
    };
    Box::into_raw(b);
    out
}