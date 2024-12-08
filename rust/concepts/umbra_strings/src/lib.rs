#![crate_name = "concepts_umbra_strings"]
mod _string;
mod _trailing;

/// An owning Umbra-styled string that does not share its bytes among different instances.
#[allow(clippy::module_name_repetitions)]
pub type BoxString<const PREFIX_LENGTH: usize = 4> =
    _string::UmbraString<_trailing::BoxDynBytes, PREFIX_LENGTH>;

/// An owning Umbra-styled string that shares its data among different instances using an atomic ref counter.
#[allow(clippy::module_name_repetitions)]
pub type ArcString<const PREFIX_LENGTH: usize = 4> =
    _string::UmbraString<_trailing::ArcDynBytes, PREFIX_LENGTH>;

/// An owning Umbra-styled string that shares its data among different instances using an plane ref counter.
#[allow(clippy::module_name_repetitions)]
pub type RcString<const PREFIX_LENGTH: usize = 4> =
    _string::UmbraString<_trailing::RcDynBytes, PREFIX_LENGTH>;
