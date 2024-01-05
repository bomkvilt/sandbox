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

# link a built target to an src tree
add_custom_command(
  TARGET main
  DEPENDS main
  COMMAND
    ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_CURRENT_BINARY_DIR}/main
      ${CMAKE_CURRENT_LIST_DIR}/main.built.link
  VERBATIM
)

# add tests
macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} PRIVATE GTest::GTest GTest::Main)

    # add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    gtest_discover_tests(
      ${TESTNAME}
      WORKING_DIRECTORY ${PROJECT_DIR}
      PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )

    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(
  generators.simple
  generators.simple/g.cpp
)
