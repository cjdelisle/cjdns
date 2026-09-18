pub mod args;
pub mod base32;
pub mod utils;

pub mod ansi {
    pub const BOLD: &str = "\x1b[1m";
    pub const BOLD_UNDERLINE: &str = "\x1b[1;4m";
    pub const CLEAR: &str = "\x1b[0m";
    pub const UNDERLINE: &str = "\x1b[4m";
}
