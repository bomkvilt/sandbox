#![crate_name = "concepts_umbra_strings"]
mod string;
mod trailing;

/// An owning Umbra-styled string that does not share its bytes among different instances.
#[allow(clippy::module_name_repetitions)]
pub type BoxString<const PREFIX_LENGTH: usize = 4> =
    string::UmbraString<trailing::BoxDynBytes, PREFIX_LENGTH>;

/// An owning Umbra-styled string that shares its data among different instances using an atomic ref counter.
#[allow(clippy::module_name_repetitions)]
pub type ArcString<const PREFIX_LENGTH: usize = 4> =
    string::UmbraString<trailing::ArcDynBytes, PREFIX_LENGTH>;

/// An owning Umbra-styled string that shares its data among different instances using an plane ref counter.
#[allow(clippy::module_name_repetitions)]
pub type RcString<const PREFIX_LENGTH: usize = 4> =
    string::UmbraString<trailing::RcDynBytes, PREFIX_LENGTH>;
