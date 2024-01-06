## ranges::views

clang::libc++ supporting statuses:
- c++20: https://libcxx.llvm.org/Status/Cxx20.html#cxx20-status
- c++23: https://libcxx.llvm.org/Status/Cxx23.html#cxx23-status


## resume

* `std::views::filter`
  * produces up to O(n depth^2) calls complexity
  * produces additional O(n d) calls complexity with `std::ranges::to`

* `std::ranges::to`
  * efficient with `std::views::transform`
