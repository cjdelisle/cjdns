use std::ffi::CString;
use std::os::raw::{c_char, c_int};

pub fn rust_main(cmain: unsafe extern "C" fn(c_int, *const *mut c_char)) {
    cjdnslog::install();
    sodiumoxide::init().unwrap();
    if std::env::var("RUST_BACKTRACE").is_err() {
        std::env::set_var("RUST_BACKTRACE", "1");
    }

    let c_args = std::env::args()
        .map(|arg| CString::new(arg).unwrap().into_raw())
        .collect::<Vec<_>>();

    tokio::runtime::Builder::new_multi_thread()
        .worker_threads(1)
        .enable_all()
        .build()
        .unwrap()
        .block_on(async { unsafe { cmain(c_args.len() as c_int, c_args.as_ptr()) } })
}

#[macro_export]
macro_rules! c_main {
    ($cmain:ident) => {
        use std::os::raw::{c_char, c_int};
        extern "C" {
            fn $cmain(argc: c_int, argv: *const *mut c_char);
        }
        fn main() {
            cjdns_sys::rust_main($cmain)
        }
    };
}

/// All the C implementations are gathered under this `external` module.
pub mod external {
    pub mod interface {
        pub mod cif;
        pub mod iface;
    }

    pub mod memory {
        pub mod allocator;
    }
}

mod interface {
    #[allow(dead_code)]
    pub mod wire {
        pub mod ethernet;
        pub mod headers;
        pub mod message;
    }

    pub mod tuntap {
        pub mod android;
    }

    pub mod rustiface_test_wrapper;
}

mod bytestring;
mod cffi;
pub mod cjdnslog;
mod crypto;
mod rffi;
mod rtypes;
mod util;
