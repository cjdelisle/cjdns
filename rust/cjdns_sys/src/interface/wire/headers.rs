//! IP headers.

pub fn get_ip_version(hdr: &[u8]) -> u8 {
    assert!(hdr.len() >= 1);
    (hdr[0] & 0xF0) >> 4
}
