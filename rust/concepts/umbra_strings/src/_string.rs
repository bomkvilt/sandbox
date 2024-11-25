use crate::_trailing;
use std::borrow::Borrow;
use std::cmp;
use std::mem::{align_of, size_of, ManuallyDrop};

// =============================================================================
// Error

#[derive(Debug)]
pub enum Error {
    /// max row length is 2**32
    TooLong,
}

impl std::error::Error for Error {}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::TooLong => write!(f, "String is too long"),
        }
    }
}

// =============================================================================
// UmbraString

const SUFFIX_LENGTH: usize = size_of::<*const u8>();

/// An owning Uumbra-styled string that does not share its bytes among different instances.
#[allow(clippy::module_name_repetitions)]
pub type BoxString<const PREFIX_LENGTH: usize = 4> =
    UmbraString<_trailing::BoxDynBytes, PREFIX_LENGTH>;

#[repr(C)]
union Trailing<Bytes: _trailing::OwnedBytes> {
    buf: [u8; SUFFIX_LENGTH],
    ptr: ManuallyDrop<Bytes>,
}

// NOTE: #[repr(C)] is required to make union poiners thin
#[allow(clippy::module_name_repetitions)]
#[repr(C)]
pub struct UmbraString<Bytes: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> {
    len: u32,
    prefix: [u8; PREFIX_LENGTH],
    trailing: Trailing<Bytes>,
}

/**
 * Public operations
 */
impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> UmbraString<B, PREFIX_LENGTH> {
    const INLINED_LENGTH: usize = PREFIX_LENGTH + SUFFIX_LENGTH;

    #[inline]
    pub const fn len(&self) -> usize {
        self.len as usize
    }

    #[inline]
    pub const fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn as_bytes(&self) -> &[u8] {
        if self.is_inlined() {
            // Safety:
            // - [repr(C)] -> `prefix` and `buf` are sequential
            unsafe {
                let buffer = std::ptr::addr_of!(self.prefix).cast::<u8>();
                std::slice::from_raw_parts(buffer, self.len())
            }
        } else {
            unsafe { self.trailing.ptr.as_bytes(self.len()) }
        }
    }

    pub fn as_str(&self) -> &str {
        // Safety:
        // - self can be built from strings only -> UTF-8
        unsafe { std::str::from_utf8_unchecked(self.as_bytes()) }
    }
}

/**
 * Byte operations.
 */
impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> UmbraString<B, PREFIX_LENGTH> {
    fn new<S: AsRef<str>, A: FnOnce(S) -> B>(s: S, alloc: A) -> Result<Self, Error> {
        const {
            // NOTE: the sizes can be allowed by declining usage of the `trailing.buf` at such cases
            assert!(
                (PREFIX_LENGTH - 4) % align_of::<Trailing<B>>() == 0,
                "PREFIX_LENGTH is not properly aligned wich means that the layot contains a gap betwean the prefix and the trailing"
            )
        };
        const { assert!(PREFIX_LENGTH < 256, "max prefix length is too large") };

        let data = s.as_ref().as_bytes();
        let size = data.len();
        if size > u32::MAX as usize {
            return Err(Error::TooLong);
        }

        let mut prefix = [0u8; PREFIX_LENGTH];

        let trailing = {
            if size <= Self::INLINED_LENGTH {
                let mut buf = [0u8; SUFFIX_LENGTH];

                if size <= PREFIX_LENGTH {
                    prefix[..size].copy_from_slice(&data[..size]);
                } else {
                    prefix.copy_from_slice(&data[..PREFIX_LENGTH]);
                    buf[..size - PREFIX_LENGTH].copy_from_slice(&data[PREFIX_LENGTH..]);
                }

                Trailing { buf }
            } else {
                prefix.copy_from_slice(&data[..PREFIX_LENGTH]);
                Trailing {
                    ptr: ManuallyDrop::new(alloc(s)),
                }
            }
        };

        #[allow(clippy::cast_possible_truncation)]
        Ok(Self {
            len: size as u32,
            prefix,
            trailing,
        })
    }

    const fn is_prefix_only(&self) -> bool {
        self.len() < PREFIX_LENGTH
    }

    const fn is_inlined(&self) -> bool {
        self.len() < Self::INLINED_LENGTH
    }

    fn suffix(&self) -> &[u8] {
        if self.is_inlined() {
            let suffix_len = self.len().saturating_sub(PREFIX_LENGTH);
            unsafe { self.trailing.buf.get_unchecked(..suffix_len) }
        } else {
            unsafe {
                self.trailing
                    .ptr
                    .as_bytes(self.len())
                    .get_unchecked(PREFIX_LENGTH..)
            }
        }
    }
}

#[inline]
fn eq_prefix<const L1: usize, const L2: usize>(lhs: &[u8; L1], rhs: &[u8; L2]) -> bool {
    let min_len: usize = cmp::min(L1, L2);
    let lhs_min = unsafe { lhs.get_unchecked(..min_len) };
    let min_rhs = unsafe { rhs.get_unchecked(..min_len) };
    lhs_min == min_rhs
}

#[inline]
fn cmp_prefix<const L1: usize, const L2: usize>(lhs: &[u8; L1], rhs: &[u8; L2]) -> cmp::Ordering {
    let min_len: usize = cmp::min(L1, L2);
    let lhs_min = unsafe { lhs.get_unchecked(..min_len) };
    let min_rhs = unsafe { rhs.get_unchecked(..min_len) };
    Ord::cmp(lhs_min, min_rhs)
}

/**
 * Symmetic operations.
 */
impl<B1: _trailing::OwnedBytes, const L1: usize> UmbraString<B1, L1> {
    fn eq<B2: _trailing::OwnedBytes, const L2: usize>(
        lhs: &UmbraString<B1, L1>,
        rhs: &UmbraString<B2, L2>,
    ) -> bool {
        // case: XXXX vs. XXXX -> cmp[XXXX]
        // case: XXX  vs. XXXX -> cmp[XXX ] -> default cmp cannot be used here
        if !eq_prefix(&lhs.prefix, &rhs.prefix) {
            return false;
        }

        // NOTE: mind XXX vs. XXXX
        if lhs.is_prefix_only() && rhs.is_prefix_only() {
            return lhs.len == rhs.len;
        }

        // NOTE: if prefixes have different size we need cannot just compare trailings
        if L1 == L2 {
            // TODO: can it be eliminated?
            if lhs.is_inlined() && rhs.is_inlined() {
                // case: XXX0 vs. XXXX -> less; (\0-padded)
                // NOTE: suffix size is always the same
                // NOTE: \0 cannot be a utf-8 rune
                return unsafe { lhs.trailing.buf == rhs.trailing.buf };
            }
            return lhs.suffix() == rhs.suffix();
        }

        // NOTE: prefices are usialy small and can be compared using a few u64-s -> no need for slicing
        return lhs.as_bytes() == rhs.as_bytes();
    }

    fn cmp<B2: _trailing::OwnedBytes, const L2: usize>(
        lhs: &UmbraString<B1, L1>,
        rhs: &UmbraString<B2, L2>,
    ) -> cmp::Ordering {
        // case: XXXX vs. XXXX -> cmp[XXXX]
        // case: XXX  vs. XXXX -> cmp[XXX ] -> default cmp cannot be used here
        let prefix_ordering = cmp_prefix(&lhs.prefix, &rhs.prefix);
        if prefix_ordering != cmp::Ordering::Equal {
            return prefix_ordering;
        }

        // NOTE: mind XXX vs. XXXX
        if lhs.is_prefix_only() && rhs.is_prefix_only() {
            return Ord::cmp(&lhs.len, &rhs.len);
        }

        // NOTE: if prefixes have different size we need cannot just compare trailings
        if L1 == L2 {
            // TODO: can it be eliminated?
            if lhs.is_inlined() && rhs.is_inlined() {
                // case: XXX0 vs. XXXX -> less; (\0-padded)
                // NOTE: suffix size is always the same
                // NOTE: \0 cannot be a utf-8 rune
                let suffix_ordering = unsafe { Ord::cmp(&lhs.trailing.buf, &rhs.trailing.buf) };
                return suffix_ordering;
            }
            return Ord::cmp(lhs.suffix(), rhs.suffix());
        }

        // NOTE: prefices are usialy small and can be compared using a few u64-s -> no need for slicing
        return Ord::cmp(lhs.as_bytes(), rhs.as_bytes());
    }
}

// =====================================
// self-related traits

/// # Safety
/// - Data is owned and immutable
unsafe impl<B: _trailing::OwnedBytes + Send, const L1: usize> Send for UmbraString<B, L1> {}

/// # Safety
/// - Data is owned and immutable
unsafe impl<B: _trailing::OwnedBytes + Sync, const L1: usize> Sync for UmbraString<B, L1> {}

impl<B: _trailing::OwnedBytes, const L1: usize> Drop for UmbraString<B, L1> {
    fn drop(&mut self) {
        if !self.is_inlined() {
            unsafe { self.trailing.ptr.drop(self.len()) };
        }
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> Clone for UmbraString<B, L1> {
    fn clone(&self) -> Self {
        let trailing = {
            if self.is_inlined() {
                unsafe {
                    Trailing {
                        buf: self.trailing.buf,
                    }
                }
            } else {
                unsafe {
                    Trailing {
                        ptr: ManuallyDrop::new(self.trailing.ptr.clone(self.len())),
                    }
                }
            }
        };
        Self {
            len: self.len,
            prefix: self.prefix,
            trailing,
        }
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> std::ops::Deref for UmbraString<B, L1> {
    type Target = str;

    fn deref(&self) -> &Self::Target {
        self.as_str()
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> AsRef<str> for UmbraString<B, L1> {
    fn as_ref(&self) -> &str {
        self.as_str()
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> Borrow<str> for UmbraString<B, L1> {
    fn borrow(&self) -> &str {
        self.as_str()
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> std::hash::Hash for UmbraString<B, L1> {
    fn hash<H: std::hash::Hasher>(&self, hasher: &mut H) {
        self.as_str().hash(hasher);
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> std::fmt::Display for UmbraString<B, L1> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.as_str())
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> std::fmt::Debug for UmbraString<B, L1> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?}", self.as_str())
    }
}

// =====================================
// TryFrom

impl<B: _trailing::OwnedBytes, const L1: usize> TryFrom<&str> for UmbraString<B, L1>
where
    B: for<'a> From<&'a [u8]>,
{
    type Error = Error;

    fn try_from(s: &str) -> Result<Self, Self::Error> {
        Self::new(s, |s| B::from(s.as_bytes()))
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> TryFrom<&String> for UmbraString<B, L1>
where
    B: for<'a> From<&'a [u8]>,
{
    type Error = Error;

    fn try_from(s: &String) -> Result<Self, Self::Error> {
        Self::new(s, |s| B::from(s.as_bytes()))
    }
}

impl<B: _trailing::OwnedBytes, const L1: usize> TryFrom<String> for UmbraString<B, L1>
where
    B: From<Vec<u8>>,
{
    type Error = Error;

    fn try_from(s: String) -> Result<Self, Self::Error> {
        Self::new(s, |s| B::from(s.into_bytes()))
    }
}

// =====================================
// Eq

// NOTE: Eq does not allow cmp strings of different inlined prefix lengths or byte types

impl<B1: _trailing::OwnedBytes, const L1: usize> Eq for UmbraString<B1, L1> {}

// =====================================
// PartialEq

// :: UmbraString vs. UmbraString

impl<B1: _trailing::OwnedBytes, B2: _trailing::OwnedBytes, const L1: usize, const L2: usize>
    PartialEq<UmbraString<B2, L2>> for UmbraString<B1, L1>
{
    fn eq(&self, other: &UmbraString<B2, L2>) -> bool {
        Self::eq(self, other)
    }
}

// :: UmbraString vs. str

impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> PartialEq<str>
    for UmbraString<B, PREFIX_LENGTH>
{
    fn eq(&self, other: &str) -> bool {
        self.as_bytes() == other.as_bytes()
    }
}

impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> PartialEq<UmbraString<B, PREFIX_LENGTH>>
    for str
{
    fn eq(&self, other: &UmbraString<B, PREFIX_LENGTH>) -> bool {
        self.as_bytes() == other.as_bytes()
    }
}

// :: UmbraString vs. String
// NOTE: `String` cannot be automatically converted to the `str`

impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> PartialEq<String>
    for UmbraString<B, PREFIX_LENGTH>
{
    fn eq(&self, other: &String) -> bool {
        self.as_bytes() == other.as_bytes()
    }
}

impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> PartialEq<UmbraString<B, PREFIX_LENGTH>>
    for String
{
    fn eq(&self, other: &UmbraString<B, PREFIX_LENGTH>) -> bool {
        self.as_bytes() == other.as_bytes()
    }
}

// =====================================
// Ord

// NOTE: Ord does not allow cmp strings of different inlined prefix lengths or byte types

impl<B1: _trailing::OwnedBytes, const L1: usize> Ord for UmbraString<B1, L1> {
    fn cmp(&self, other: &UmbraString<B1, L1>) -> cmp::Ordering {
        Self::cmp(self, other)
    }
}

// =====================================
// PartialOrd

// :: UmbraString vs. UmbraString

impl<B1: _trailing::OwnedBytes, B2: _trailing::OwnedBytes, const L1: usize, const L2: usize>
    PartialOrd<UmbraString<B2, L2>> for UmbraString<B1, L1>
{
    fn partial_cmp(&self, other: &UmbraString<B2, L2>) -> Option<cmp::Ordering> {
        Some(Self::cmp(self, other))
    }
}

// :: UmbraString vs. str

impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> PartialOrd<str>
    for UmbraString<B, PREFIX_LENGTH>
{
    fn partial_cmp(&self, other: &str) -> Option<cmp::Ordering> {
        PartialOrd::partial_cmp(self.as_bytes(), other.as_bytes())
    }
}

impl<B: _trailing::OwnedBytes, const PREFIX_LENGTH: usize> PartialOrd<UmbraString<B, PREFIX_LENGTH>>
    for str
{
    fn partial_cmp(&self, other: &UmbraString<B, PREFIX_LENGTH>) -> Option<cmp::Ordering> {
        PartialOrd::partial_cmp(self.as_bytes(), other.as_bytes())
    }
}
