use super::base32;
use eyre::{eyre, bail, Result};
use sha2::{
    digest::{Digest, Output},
    Sha512,
};
use std::{env, fmt::Write, path::MAIN_SEPARATOR};

pub fn exe_name() -> String {
    env::args()
        .next()
        .and_then(|path| {
            path.rsplit(MAIN_SEPARATOR)
                .next()
                .filter(|exe| !exe.is_empty())
                .map(str::to_owned)
        })
        .unwrap_or_else(|| "cjdnstool".to_owned())
}

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

pub fn key_to_ip6(with_key: &str, with_prefix: bool) -> Result<String> {
    if with_key.ends_with(".k") {
        let mut key = &with_key[..(with_key.len() - 2)];
        let prefix;
        if with_prefix {
            let (l, r) = key
                .rsplit_once('.')
                .ok_or_else(|| eyre!("expected prefix before key"))?;
            prefix = Some(l);
            key = r;
        } else {
            prefix = None;
        }
        let raw_key =
            base32::decode(key.as_bytes()).map_err(|e| eyre!("invalid key format: {}", e))?;
        let ipv6 = hash_to_ip6(Sha512::digest(Sha512::digest(raw_key)));
        Ok(if let Some(prefix) = prefix {
            format!("{prefix}.{ipv6}")
        } else {
            ipv6
        })
    } else {
        bail!("invalid key format: missing \".k\" suffix")
    }
}

pub fn hash_to_ip6(hash: Output<Sha512>) -> String {
    const IP6_LEN: usize = 39;

    let mut ip6 = String::with_capacity(IP6_LEN);
    for chunk in hash.chunks(2).take(8) {
        if !ip6.is_empty() {
            ip6.push(':');
        }
        write!(ip6, "{:02x}{:02x}", chunk[0], chunk[1]).unwrap();
    }
    ip6
}

#[cfg(test)]
mod test {
    fn test_key_to_ip6_samples(samples: &[(&str, &str)], with_prefix: bool) {
        for (&ref key, &ref ip6) in samples {
            assert_eq!(super::key_to_ip6(key, with_prefix).unwrap(), ip6);
        }
    }

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
        ];
        test_key_to_ip6_samples(SAMPLES, false)
    }

    #[test]
    fn test_key_to_ip6_with_prefix() {
        const SAMPLES: &[(&str, &str)] = &[
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
        test_key_to_ip6_samples(SAMPLES, true)
    }
}
