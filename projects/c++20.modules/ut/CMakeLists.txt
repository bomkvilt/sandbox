add_executable(c++20.modules.ut)

target_sources(
  c++20.modules.ut
  PRIVATE
    FILE_SET modules TYPE CXX_MODULES FILES
      tests.cpp
)

gn3_target_link_binary_to_src(c++20.modules.ut)
gn3_target_gtest_configure(c++20.modules.ut)

target_link_libraries(c++20.modules.ut PRIVATE c++20.modules)
