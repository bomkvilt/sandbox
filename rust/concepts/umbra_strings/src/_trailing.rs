use std::alloc::Layout;
use std::ptr::NonNull;

/// Trait for trailing bytes, which does not store their actual legth -> has thin pointers
/// # Safety
/// - the buffer does not store its actual size
pub unsafe trait Bytes: Send + Sync {
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

// =============================================================================

#[repr(C)]
pub struct BoxDynBytes {
    ptr: NonNull<u8>,
}

// NOTE: Data is owned
unsafe impl Send for BoxDynBytes {}

// NOTE: Data is immutable
unsafe impl Sync for BoxDynBytes {}

impl From<Vec<u8>> for BoxDynBytes {
    fn from(bytes: Vec<u8>) -> Self {
        let ptr = {
            if bytes.is_empty() {
                NonNull::dangling()
            } else {
                // `Box::into_raw` is guaranteed to be non-null and aligned
                unsafe { NonNull::new_unchecked(Box::into_raw(bytes.into_boxed_slice()).cast()) }
            }
        };
        Self { ptr }
    }
}

impl From<&[u8]> for BoxDynBytes {
    fn from(bytes: &[u8]) -> Self {
        let ptr = {
            if bytes.is_empty() {
                NonNull::dangling()
            } else {
                let layout = array_layout::<u8>(bytes.len());

                let nullable = unsafe { std::alloc::alloc(layout) };
                let Some(ptr) = NonNull::new(nullable) else {
                    std::alloc::handle_alloc_error(layout);
                };

                unsafe {
                    std::ptr::copy_nonoverlapping(bytes.as_ptr(), ptr.as_ptr(), bytes.len());
                }

                ptr
            }
        };
        Self { ptr }
    }
}

unsafe impl Bytes for BoxDynBytes {
    unsafe fn as_bytes(&self, len: usize) -> &[u8] {
        if len == 0 {
            Default::default()
        } else {
            std::slice::from_raw_parts(self.ptr.as_ptr(), len)
        }
    }

    unsafe fn clone(&self, len: usize) -> Self {
        // NOTE: can be written more optimal but more verbose
        Self::from(self.as_bytes(len))
    }
}

unsafe impl OwnedBytes for BoxDynBytes {
    unsafe fn drop(&self, len: usize) {
        if len > 0 {
            unsafe {
                std::alloc::dealloc(self.ptr.as_ptr(), array_layout::<u8>(len));
            }
        }
    }
}

// =============================================================================
// layouts

fn array_layout<T>(len: usize) -> Layout {
    Layout::array::<T>(len).expect("Failed to create array layout")
}
