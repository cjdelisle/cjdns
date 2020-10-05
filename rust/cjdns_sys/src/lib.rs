#[macro_export]
macro_rules! c_main {
    ($cmain:ident) => {
        use std::ffi::CString;
        use std::os::raw::c_char;
        use std::os::raw::c_int;
        extern "C" {
            fn $cmain(argc: c_int, argv: *const *mut c_char);
        }
        fn main() {
            sodiumoxide::init().unwrap();
            let c_args = std::env::args()
                .map(|arg| CString::new(arg).unwrap())
                .map(|arg| arg.into_raw())
                .collect::<Vec<*mut c_char>>();
            unsafe { $cmain(c_args.len() as c_int, c_args.as_ptr()) }
        }
    };
}
