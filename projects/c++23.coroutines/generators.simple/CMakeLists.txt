set(NAME c++23.coro.generators.simple)
add_executable(
    ${NAME}
    main.cpp
)
gn3_target_link_binary_to_src(${NAME})
gn3_target_gtest_configure(${NAME})
