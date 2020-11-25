//! Logger from C part of the project.

/// External opaque C logger type. Should be used in a form of a pointer only.
#[repr(C)]
pub struct Log {
    _dummy: usize,
}
