set(NAME c++23.coro.generators.iterable)
add_executable(
    ${NAME}
    generators.iterable.cpp
)
gn3_target_link_binary_to_src(${NAME})
gn3_target_gtest_configure(${NAME})
