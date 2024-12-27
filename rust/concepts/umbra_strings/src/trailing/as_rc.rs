use std::alloc::Layout;
use std::cell::Cell;
use std::ptr::NonNull;

use super::traits;

#[repr(C)]
struct RcDynBytesData<T: ?Sized> {
    count: Cell<usize>,
    data: T,
}

impl<T> RcDynBytesData<[T]> {
    fn from(ptr: *mut T, len: usize) -> *mut Self {
        // NOTE: it's safe to pass a null pointer to the method, however it's not safe to read it
        let fake_slice = std::ptr::slice_from_raw_parts_mut(ptr, len);
        // NOTE: de-facto it is a reinterpret cast; which works
        fake_slice as *mut Self
    }
}

// ===========================

#[repr(C)]
pub struct RcDynBytes {
    ptr: NonNull<RcDynBytesData<[u8; 0]>>, // NOTE: array == thin pointer, actual data is a C pointer
}

impl RcDynBytes {
    fn layout(len: usize) -> Layout {
        Layout::new::<RcDynBytesData<()>>()
            .extend(array_layout::<u8>(len))
            .unwrap()
            .0
            .pad_to_align()
    }
}

// NOTE: the bytes cannot be used by multiple threads
// unsafe impl Send for RcDynBytes {}

// NOTE: the bytes cannot be used by multiple threads
// unsafe impl Sync for RcDynBytes {}

impl From<Vec<u8>> for RcDynBytes {
    fn from(bytes: Vec<u8>) -> Self {
        // NOTE: the bytes should be copied any way
        Self::from(&bytes[..])
    }
}

impl From<&[u8]> for RcDynBytes {
    fn from(bytes: &[u8]) -> Self {
        if bytes.is_empty() {
            return Self {
                ptr: NonNull::dangling(),
            };
        }

        let layout = Self::layout(bytes.len());

        // NOTE: size of the `data` != 0 due to the `if` above
        let nullable = unsafe { std::alloc::alloc(layout) };
        let nullable_fat_ptr = RcDynBytesData::<[u8]>::from(nullable, bytes.len());
        let Some(fat_ptr) = NonNull::new(nullable_fat_ptr) else {
            std::alloc::handle_alloc_error(layout);
        };

        // NOTE: we have just allocated a required memory strip
        // NOTE: I cannot explain why `fat_ptr.as_mut()` cannot be used here
        unsafe {
            std::ptr::write(
                std::ptr::addr_of_mut!((*fat_ptr.as_ptr()).count),
                Cell::new(1),
            );
            std::ptr::copy_nonoverlapping(
                bytes.as_ptr(),
                std::ptr::addr_of_mut!((*fat_ptr.as_ptr()).data).cast(),
                bytes.len(),
            );
        }
        Self {
            ptr: fat_ptr.cast(),
        }
    }
}

unsafe impl traits::Bytes for RcDynBytes {
    unsafe fn as_bytes(&self, len: usize) -> &[u8] {
        if len == 0 {
            Default::default()
        } else {
            let fat_ptr = RcDynBytesData::<[u8]>::from(self.ptr.as_ptr().cast(), len);
            &(*fat_ptr).data
        }
    }

    unsafe fn clone(&self, len: usize) -> Self {
        let ptr = {
            if len == 0 {
                NonNull::dangling()
            } else {
                // NOTE: as_mut cannot be used here since we have a &self, not &mut self
                let data = unsafe { &mut *self.ptr.as_ptr() };
                *data.count.get_mut() += 1;
                self.ptr
            }
        };
        Self { ptr }
    }
}

unsafe impl traits::OwnedBytes for RcDynBytes {
    unsafe fn drop(&self, len: usize) {
        if len > 0 {
            let raw = self.ptr.as_ptr();
            *(*raw).count.get_mut() -= 1;
            if (*raw).count.get() == 1 {
                // NOTE: the dynamic part of the `data` is alreadt counted by the `layout`
                unsafe { std::alloc::dealloc(raw.cast::<u8>(), Self::layout(len)) };
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
