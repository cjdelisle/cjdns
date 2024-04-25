use std::marker::PhantomData;

pub trait Ctx: 'static + Clone + Send + Sync {}
impl<X> Ctx for X where X: 'static + Clone + Send + Sync {}

pub trait Arg: 'static + Send + Sync {}
impl<X> Arg for X where X: 'static + Send + Sync {}

pub trait Out: 'static + Send + Sync {}
impl<X> Out for X where X: 'static + Send + Sync {}

pub trait Fun<C: Ctx, T: Arg, O: Out>: 'static + Fn(C, T) -> O + Send + Sync {}
impl<X: 'static + Fn(C, T) -> O + Send + Sync, C: Ctx, T: Arg, O: Out> Fun<C,T,O> for X {}

pub trait Callable<T: Arg, O: 'static + Send + Sync>: Send + Sync {
    fn call(&self, t: T) -> O;
}
impl<T: Arg, O: Out> dyn Callable<T, O> {
    pub fn new<C: Ctx, F: Fun<C,T,O>>(ctx: C, f: F) -> impl Callable<T, O> {
        CtxCallable {
            ctx,
            f,
            _t: PhantomData {},
            _o: PhantomData {},
        }
    }
}

struct CtxCallable<C: Ctx, T: Arg, F: Fun<C,T,O>, O: Out> {
    ctx: C,
    f: F,
    _t: PhantomData<T>,
    _o: PhantomData<O>,
}
impl<C: Ctx, T: Arg, F: Fun<C,T,O>, O: Out> Callable<T, O> for CtxCallable<C, T, F, O> {
    fn call(&self, t: T) -> O {
        (self.f)(self.ctx.clone(), t)
    }
}