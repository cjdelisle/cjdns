#[no_mangle]
pub extern "C" fn Rffi_parseBase10(
    buf: *const u8,
    max_len: u32,
    num_out: *mut i64,
    bytes: *mut u32,
) -> libc::c_int {
    // Safety: Ensure that buf is valid and not null
    if buf.is_null() {
        return -1; // Indicates error: null pointer
    }

    // Safety: Ensure that num_out is valid and not null
    if num_out.is_null() {
        return -2; // Indicates error: null pointer
    }

    // Convert the buffer into a slice
    let buf_slice = unsafe { std::slice::from_raw_parts(buf, max_len as usize) };

    let mut limit = buf_slice.len();
    for (i, &c) in buf_slice.iter().enumerate() {
        if c != b'-' && (c < b'0' || c > b'9') {
            limit = i;
            break;
        }
    }

    // Convert the slice into a string
    let buf_str = match std::str::from_utf8(&buf_slice[0..limit]) {
        Ok(s) => s,
        Err(_) => return -3, // Indicates error: invalid UTF-8 sequence
    };

    // Parse the string into an i64
    let parsed_num = match buf_str.parse::<i64>() {
        Ok(n) => n,
        Err(_) => return -4, // Indicates error: unable to parse as i64
    };

    // Assign the parsed number to num_out
    unsafe {
        *num_out = parsed_num;
        *bytes = limit as _;
    };

    0 // Indicates success
}
