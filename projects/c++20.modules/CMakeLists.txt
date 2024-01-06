add_subdirectory(ut)

add_library(c++20.modules)

target_sources(
  c++20.modules
  PUBLIC
    # NOTE: list of module files that expose `export module <>;` only
    FILE_SET public_modules TYPE CXX_MODULES FILES
    module.cppm
    submodule/a.cppm
  PRIVATE
    # NOTE: implementation units and ordinary .cpp files
    module.cpp
    submodule/a.cpp
)
