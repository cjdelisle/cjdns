use std::future::Future;
use std::marker::PhantomData;

/// Trait type for the Context argument, you must implement 'static + Clone + Send + Sync
pub trait Ctx: 'static + Clone + Send + Sync {}
impl<X> Ctx for X where X: 'static + Clone + Send + Sync {}

/// Trait type for Arg argument, you must implement 'static + Send + Sync
pub trait Arg: 'static + Send + Sync {}
impl<X> Arg for X where X: 'static + Send + Sync {}

/// Trait type for function, implements 'static + Fn(C,T) -> Self::Fut + Send + Sync
pub trait Fun<C: Ctx, T: Arg, O: Out>: 'static + Fn(C, T) -> Self::Fut + Send + Sync {
    type Fut: Fut<O>;
}
impl<X, C, T, U, O> Fun<C, T, O> for X
where
    X: 'static + Fn(C, T) -> U + Send + Sync,
    C: Ctx,
    T: Arg,
    U: Fut<O>,
    O: Out,
{
    type Fut = U;
}

/// Trait type for the direct Fun output, implements 'static + Future<Output = O> + Send + Sync
pub trait Fut<O: Out>: 'static + Future<Output = O> + Send + Sync {}
impl<X, O> Fut<O> for X
where
    X: 'static + Future<Output = O> + Send + Sync,
    O: Out,
{
}

/// Trait type for the final Fun output, implements 'static + Send + Sync
pub trait Out: 'static + Send + Sync {}
impl<X> Out for X where X: 'static + Send + Sync {}

/// Callable trait, you call with Arg type T but context type is captured/hidden in the implementation
#[async_trait::async_trait]
pub trait AsyncCallable<T: Arg, O: Out>: Send + Sync {
    async fn call(&self, t: T) -> O;
}
impl<T: Arg, O: Out> dyn AsyncCallable<T, O> {
    /// Create a new Callable
    /// Generics:
    ///   C: The type of the Context (first arg) - Must implement Ctx, i.e. 'static + Clone + Send + Sync
    ///   T: Type of the (second) arg to the function - Must implement Arg, i.e. 'static + Send + Sync
    ///   F: The function: async fn(C,T) -> O
    ///   O: The (async) output of the function - Must implement Out, i.e. 'static + Send + Sync
    /// Args:
    ///   ctx: The context (Ctx)
    ///   f: The function
    /// ```rust
    /// async fn xxx(ctx: Ctx, msg: String) {
    ///     println!("num = {}, msg = {}", ctx.num, msg);
    /// }
    /// async fn test_fn() {
    ///     let ctx = Ctx{ num: 3};
    ///     let callable = super::new(ctx, xxx);
    ///     callable.call("hi".into()).await;
    /// }
    /// ```
    fn new<C: Ctx, F: Fun<C,T,O>>(ctx: C, f: F) -> impl AsyncCallable<T, O> {
        CtxCallable {
            ctx,
            f,
            _t: PhantomData {},
            _o: PhantomData {},
        }
    }
}

struct CtxCallable<C, T, F, O>
where
    C: Ctx,
    T: Arg,
    F: Fun<C, T, O>,
    O: 'static + Send + Sync,
{
    ctx: C,
    f: F,
    _t: PhantomData<T>,
    _o: PhantomData<O>,
}
#[async_trait::async_trait]
impl<C, T, F, O> AsyncCallable<T, O> for CtxCallable<C, T, F, O>
where
    C: Ctx,
    T: Arg,
    F: Fun<C, T, O>,
    O: 'static + Send + Sync,
{
    async fn call(&self, t: T) -> O {
        (self.f)(self.ctx.clone(), t).await
    }
}

#[cfg(test)]
mod tests {
    use super::AsyncCallable;
    #[derive(Clone)]
    struct Ctx {
        num: usize,
    }

    #[tokio::test]
    async fn test_callable() {
        let ctx = Ctx { num: 3 };
        let callable = AsyncCallable::new(ctx, |ctx, msg: String| async move {
            println!("num = {}, msg = {}", ctx.num, msg);
        });
        callable.call("hi".into()).await;
    }

    async fn xxx(ctx: Ctx, msg: String) {
        println!("num = {}, msg = {}", ctx.num, msg);
    }
    #[tokio::test]
    async fn test_fn() {
        let ctx = Ctx { num: 3 };
        let callable = AsyncCallable::new(ctx, xxx);
        callable.call("hi".into()).await;
    }
}
