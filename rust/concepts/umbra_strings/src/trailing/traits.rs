/// Trait for trailing bytes, which does not store their actual legth -> has thin pointers
/// # Safety
/// - the buffer does not store its actual size
pub unsafe trait Bytes {
    /// # Safety
    /// - the buffer does not store its actual size
    #[must_use]
    unsafe fn as_bytes(&self, len: usize) -> &[u8];

    /// # Safety
    /// - the buffer does not store its actual size
    #[must_use]
    unsafe fn clone(&self, len: usize) -> Self;
}

/// Trait for owned trailing bytes
/// # Safety
/// - the buffer does not store its actual size
pub unsafe trait OwnedBytes: Bytes {
    /// # Safety
    /// - the buffer does not store its actual size
    unsafe fn drop(&self, len: usize);
}
