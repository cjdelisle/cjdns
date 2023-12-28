use serde::{Deserialize, Deserializer};

pub fn as_bool<'de, D: Deserializer<'de>>(deserializer: D) -> Result<bool, D::Error> {
    i32::deserialize(deserializer).map(|i| i != 0)
}
