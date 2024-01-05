cmake_minimum_required(VERSION 3.28)

# Create a library
add_library(foo)
target_sources(foo
  PUBLIC
    FILE_SET CXX_MODULES FILES
      a.cppm
)

# Create an executable
add_executable(main main.cpp)

# Link to the library foo
target_link_libraries(main foo)
gn3_target_link_binary_to_src(main)


# add tests
add_executable(generators.simple generators.simple/g.cpp)
gn3_target_link_binary_to_src(generators.simple)
gn3_target_gtest_configure(generators.simple)
