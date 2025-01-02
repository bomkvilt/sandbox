set(NAME c++23.coro.generators.simple)
add_executable(
    ${NAME}
    generators.simple.cpp
)
gn3_target_link_binary_to_src(${NAME})
gn3_target_gtest_configure(${NAME})
