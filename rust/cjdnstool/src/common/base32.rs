use data_encoding::{BitOrder, DecodeError, Encoding, Specification, Translate, Wrap};
use lazy_static::lazy_static;

lazy_static! {
    static ref BASE32: Encoding = Specification {
        symbols: "0123456789bcdfghjklmnpqrstuvwxyz".to_owned(),
        bit_order: BitOrder::LeastSignificantFirst,
        check_trailing_bits: true,
        padding: None,
        ignore: String::new(),
        wrap: Wrap {
            width: 0,
            separator: String::new()
        },
        translate: Translate {
            from: "BCDFGHJKLMNPQRSTUVWXYZ".to_owned(),
            to: "bcdfghjklmnpqrstuvwxyz".to_owned(),
        },
    }
    .encoding()
    .expect("invalid encoding specification");
}

pub fn decode(input: &[u8]) -> Result<Vec<u8>, DecodeError> {
    BASE32.decode(input)
}

pub fn encode(input: &[u8]) -> String {
    BASE32.encode(input)
}
