cmake_minimum_required(VERSION 3.28)

project(async_cpp LANGUAGES CXX)

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

# link built target to cmake an src tree
add_custom_command(
  TARGET main
  DEPENDS main
  COMMAND
    ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_CURRENT_BINARY_DIR}/main
      ${CMAKE_CURRENT_LIST_DIR}/main.built.link
  VERBATIM
)
