use std::alloc::Layout;
use std::cell::Cell;
use std::ptr::NonNull;
use std::sync::atomic::{self, AtomicUsize};

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

// =============================================================================

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
                std::alloc::dealloc(self.ptr.as_ptr(), Self::layout(len));
            }
        }
    }
}

// =============================================================================

#[repr(C)]
struct ArcDynBytesData<T: ?Sized> {
    count: AtomicUsize,
    data: T,
}

impl<T> ArcDynBytesData<[T]> {
    fn from(ptr: *mut T, len: usize) -> *mut Self {
        // NOTE: it's safe to pass a null pointer to the method, however it's not safe to read it
        let fake_slice = std::ptr::slice_from_raw_parts_mut(ptr, len);
        // NOTE: de-facto it is a reinterpret cast; wich works
        fake_slice as *mut Self
    }
}

// ===========================

#[repr(C)]
pub struct ArcDynBytes {
    ptr: NonNull<ArcDynBytesData<[u8; 0]>>, // NOTE: array == thin pointer, actual data is a C pointer
}

impl ArcDynBytes {
    fn layout(len: usize) -> Layout {
        Layout::new::<ArcDynBytesData<()>>()
            .extend(array_layout::<u8>(len))
            .unwrap()
            .0
            .pad_to_align()
    }
}

// NOTE: Data is owned, ref. counter is atomic
unsafe impl Send for ArcDynBytes {}

// NOTE: Data is immutable
unsafe impl Sync for ArcDynBytes {}

impl From<Vec<u8>> for ArcDynBytes {
    fn from(bytes: Vec<u8>) -> Self {
        // NOTE: the bytes should be copied any way
        Self::from(&bytes[..])
    }
}

impl From<&[u8]> for ArcDynBytes {
    fn from(bytes: &[u8]) -> Self {
        if bytes.is_empty() {
            return Self {
                ptr: NonNull::dangling(),
            };
        }

        let layout = Self::layout(bytes.len());

        // NOTE: size of the `data` != 0 due to the `if` above
        let nullable = unsafe { std::alloc::alloc(layout) };
        let nullable_fat_ptr = ArcDynBytesData::<[u8]>::from(nullable, bytes.len());
        let Some(fat_ptr) = NonNull::new(nullable_fat_ptr) else {
            std::alloc::handle_alloc_error(layout);
        };

        // NOTE: we have just allocated a required memory strip
        // NOTE: I cannot explain why `fat_ptr.as_mut()` cannot be used here
        unsafe {
            std::ptr::write(
                std::ptr::addr_of_mut!((*fat_ptr.as_ptr()).count),
                AtomicUsize::new(1),
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

unsafe impl Bytes for ArcDynBytes {
    unsafe fn as_bytes(&self, len: usize) -> &[u8] {
        if len == 0 {
            Default::default()
        } else {
            let fat_ptr = ArcDynBytesData::<[u8]>::from(self.ptr.as_ptr().cast(), len);
            &(*fat_ptr).data
        }
    }

    unsafe fn clone(&self, len: usize) -> Self {
        let ptr = {
            if len == 0 {
                NonNull::dangling()
            } else {
                // NOTE: as_mut cannot be used here since we have a &self, not &mut self
                let data = unsafe { &*self.ptr.as_ptr() };
                // NOTE: here we need just atomicy of the changes
                data.count.fetch_add(1, atomic::Ordering::Relaxed);
                self.ptr
            }
        };
        Self { ptr }
    }
}

unsafe impl OwnedBytes for ArcDynBytes {
    unsafe fn drop(&self, len: usize) {
        if len > 0 {
            let raw = self.ptr.as_ptr();
            // NOTE: here we need to ensure that critical section operations do not float above the check; but do we?
            if (*raw).count.fetch_sub(1, atomic::Ordering::Acquire) == 1 {
                // NOTE: the dynamic part of the `data` is alreadt counted by the `layout`
                unsafe { std::alloc::dealloc(raw.cast::<u8>(), Self::layout(len)) };
            }
        }
    }
}

// =============================================================================

#[repr(C)]
struct RcDynBytesData<T: ?Sized> {
    count: Cell<usize>,
    data: T,
}

impl<T> RcDynBytesData<[T]> {
    fn from(ptr: *mut T, len: usize) -> *mut Self {
        // NOTE: it's safe to pass a null pointer to the method, however it's not safe to read it
        let fake_slice = std::ptr::slice_from_raw_parts_mut(ptr, len);
        // NOTE: de-facto it is a reinterpret cast; wich works
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

unsafe impl Bytes for RcDynBytes {
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

unsafe impl OwnedBytes for RcDynBytes {
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
