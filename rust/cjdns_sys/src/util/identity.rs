use std::{any::TypeId, marker::PhantomData};

pub struct Identity<T:'static>(u64, PhantomData<T>);
impl <T:'static> Identity<T> {
    fn type_id() -> u64 {
        let tid = TypeId::of::<T>();
        unsafe { 
            let x: u128 = std::mem::transmute(tid);
            x as u64
        }
    }
    pub fn check(&self) {
        assert_eq!(self.0, Self::type_id());
    }
    pub fn new() -> Self {
        Self(Self::type_id(),PhantomData::default())
    }
}
impl<T> Default for Identity<T> {
    fn default() -> Self {
        Self::new()
    }
}

macro_rules! from_c {
    ($obj:expr) => {
        {
            let o = $obj;
            unsafe {
                (*o).identity.check();
                &mut (*o)
            }
        }
    };
}
pub(crate) use from_c;