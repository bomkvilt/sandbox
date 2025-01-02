#include "gtest/gtest.h"

#include <algorithm>
#include <functional>
#include <ranges>
#include <vector>


#ifndef ERROR
    #define ERROR 0
#endif


namespace {
    struct S {
        int X;
        int Y;
    };
}


TEST(cpp23_views, borrow_checher) {
    std::vector<S> w = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};

#if ERROR
    auto it4 = std::ranges::find(w | std::views::transform(std::mem_fn(&S::X)), 4).base();
    GTEST_ASSERT_EQ(*it4, 4);

    auto it5 = std::ranges::find(std::views::all(w) | std::views::transform(std::mem_fn(&S::X)), 4).base();
    GTEST_ASSERT_EQ(*it5, 4);
#endif

    auto tm6 = std::views::all(w) | std::views::transform(std::mem_fn(&S::X));
    auto it6 = std::ranges::find(tm6, 4).base();
    GTEST_ASSERT_EQ(it6->X, 4);
}

#undef ERROR
