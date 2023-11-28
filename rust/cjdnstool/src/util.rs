use anyhow::{anyhow, bail, Result};
use data_encoding::{BitOrder, Encoding, Specification, Translate, Wrap};
use lazy_static::lazy_static;
use sha2::{Digest, Sha512};
use std::iter;

pub trait PushField {
    fn push_field(&mut self, field: impl AsRef<str>);
}

impl PushField for String {
    fn push_field(&mut self, field: impl AsRef<str>) {
        if !self.is_empty() {
            self.push(' ');
        }
        self.push_str(field.as_ref());
    }
}

pub fn print_padded<const N: usize>(lines: Vec<[String; N]>) {
    let mut paddings = [0; N];
    for cols in &lines {
        for i in 0..N.saturating_sub(1) {
            let pad = &mut paddings[i];
            let chars = cols[i].chars().count();
            if chars > *pad {
                *pad = chars;
            }
        }
    }

    for mut cols in lines {
        let mut last_non_empty = 0;
        for i in (0..N).rev() {
            let col = &mut cols[i];
            if i < last_non_empty {
                let pad = paddings[i] - col.chars().count();
                if pad > 0 {
                    col.reserve(pad);
                    for _ in 0..pad {
                        col.push(' ');
                    }
                }
            } else if !col.is_empty() {
                last_non_empty = i;
            }
        }
        println!("{}", cols[..=last_non_empty].join(" "));
    }
}

pub fn key_to_ip6(with_key: &str) -> Result<String> {
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

    if with_key.ends_with(".k") {
        let mut key = &with_key[..(with_key.len() - 2)];
        let left = match key.rsplit_once('.') {
            Some((l, r)) => {
                key = r;
                Some(l)
            }
            _ => None,
        };
        let bytes = BASE32
            .decode(key.as_bytes())
            .map_err(|e| anyhow!("invalid key format: {}", e))?;
        let hash = format!("{:x}", Sha512::digest(Sha512::digest(bytes)));
        let ipv6 = hash
            .chars()
            .take(32)
            .enumerate()
            .flat_map(|(i, c)| {
                if i != 0 && i % 4 == 0 {
                    Some(':')
                } else {
                    None
                }
                .into_iter()
                .chain(iter::once(c))
            })
            .collect();
        Ok(if let Some(left) = left {
            format!("{left}.{ipv6}")
        } else {
            ipv6
        })
    } else {
        bail!("invalid key format: missing \".k\" suffix")
    }
}

#[cfg(test)]
mod test {
    #[test]
    fn test_key_to_ip6() {
        const SAMPLES: &[(&str, &str)] = &[
            (
                "rjndc8rvg194ddf2j5v679cfjcpmsmhv8p022q3lvpym21cqwyh0.k",
                "fc50:47a8:2ef5:1c82:952e:10fc:dbad:dba9",
            ),
            (
                "RJNDC8RVG194DDF2J5V679CFJCPMSMHV8P022Q3LVPYM21CQWYH0.k",
                "fc50:47a8:2ef5:1c82:952e:10fc:dbad:dba9",
            ),
            (
                "v21.0000.0000.0000.001d.08bz912l989nzqc21q9x5qr96ns465nd71f290hb9q40z94jjw60.k",
                "v21.0000.0000.0000.001d.fc8d:56ed:a8f3:237e:e586:2447:9966:9be1",
            ),
            (
                "v20.0000.0000.0000.001b.byxcwmgbhkcgt3vv2820vujbc65szwkn9sj7vk1x3tjdw4q0sc30.k",
                "v20.0000.0000.0000.001b.fcb6:19d6:8d6a:7437:0213:039c:d9fb:e255",
            ),
            (
                "v20.0000.0000.0000.0019.kw0vfw3tmb6u6p21z5jmmymdlumwknlg3x8muk5mcw66tdpqlw30.k",
                "v20.0000.0000.0000.0019.fc02:2735:e595:bb70:8ffc:5293:8af8:c4b7",
            ),
        ];
        for (&ref key, &ref ip6) in SAMPLES {
            assert_eq!(super::key_to_ip6(key).unwrap(), ip6);
        }
    }
}
