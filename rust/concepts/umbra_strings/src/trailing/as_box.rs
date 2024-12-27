use std::alloc::Layout;
use std::ptr::NonNull;

use super::traits;

#[repr(C)]
pub struct BoxDynBytes {
    ptr: NonNull<u8>,
}

impl BoxDynBytes {
    fn layout(len: usize) -> Layout {
        array_layout::<u8>(len)
    }
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
                let layout = Self::layout(bytes.len());

                let nullable = unsafe { std::alloc::alloc(layout) };
                let Some(ptr) = NonNull::new(nullable) else {
                    std::alloc::handle_alloc_error(layout);
                };

                unsafe { std::ptr::copy_nonoverlapping(bytes.as_ptr(), ptr.as_ptr(), bytes.len()) };

                ptr
            }
        };
        Self { ptr }
    }
}

unsafe impl traits::Bytes for BoxDynBytes {
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

unsafe impl traits::OwnedBytes for BoxDynBytes {
    unsafe fn drop(&self, len: usize) {
        if len > 0 {
            unsafe {
                std::alloc::dealloc(self.ptr.as_ptr(), Self::layout(len));
            }
        }
    }
}

// =============================================================================
// layouts

fn array_layout<T>(len: usize) -> Layout {
    Layout::array::<T>(len).expect("Failed to create array layout")
}

// =============================================================================
// tests

#[cfg(test)]
mod tests {
    // use super::{ArcDynBytes, BoxDynBytes, RcDynBytes};

    #[test]
    fn test() {
        // TODO: add tests
    }
}
