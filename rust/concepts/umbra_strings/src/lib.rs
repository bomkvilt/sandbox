#![crate_name = "concepts_umbra_strings"]
mod _string;
mod _trailing;

pub type BoxString<const PREFIX_LENGTH: usize = 4> = _string::BoxString<PREFIX_LENGTH>;
