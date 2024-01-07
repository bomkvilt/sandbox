add_executable(
    c++23.views
    views.cpp
)
gn3_target_link_binary_to_src(c++23.views)
gn3_target_gtest_configure(c++23.views)
