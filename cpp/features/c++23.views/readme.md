# ranges::views

clang::libc++ supporting statuses:

- c++20: <https://libcxx.llvm.org/Status/Cxx20.html#cxx20-status>
- c++23: <https://libcxx.llvm.org/Status/Cxx23.html#cxx23-status>

leactures:

- <https://www.youtube.com/watch?v=sfweY8osTEs>
  - 1:23:00 : requires is a hook to name resolving process

- <https://www.youtube.com/watch?v=CVMDB3c3too>
  - 0:03:15 : std::invoke(obj, field) == std::mem_fn(field)(obj)
  - 0:11:08 : std::ranges::dangling, std::enable_borrowed_range, std::borrowed_range

## summary

- `std::views::filter`
  - produces up to O(n depth^2) calls complexity
  - produces additional O(n d) calls complexity with `std::ranges::to`

- `std::ranges::to`
  - efficient with `std::views::transform`
