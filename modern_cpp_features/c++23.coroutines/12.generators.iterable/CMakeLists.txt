# ----------------------------------------------------------
# test
# ----------------------------------------------------------

set(NAME c++23.coro.generators.iterable)
add_executable(${NAME})
target_sources(
  ${NAME}
  PRIVATE
    FILE_SET private_modules TYPE CXX_MODULES FILES
    generators.iterable.cpp
    generator_v2.cppm
)
gn3_target_link_binary_to_src(${NAME})
gn3_target_gtest_configure(${NAME})


# ----------------------------------------------------------
# benchmarks
# ----------------------------------------------------------

set(NAME c++23.coro.generators.iterable.bench)
add_executable(${NAME})
target_sources(
  ${NAME}
  PRIVATE
    FILE_SET private_modules TYPE CXX_MODULES FILES
    generators.iterable.bench.cpp
    generator_v2.cppm
)
gn3_target_link_binary_to_src(${NAME})
gn3_target_gbench_configure(${NAME})
