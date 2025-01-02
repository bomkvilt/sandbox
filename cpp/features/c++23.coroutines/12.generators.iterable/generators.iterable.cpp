module;

#include "gtest/gtest.h"

#include <coroutine>
#include <ranges>


// NOTE: clangd has incompleat support of modules
// SA: https://github.com/clangd/clangd/issues/1293
// SA: https://github.com/clangd/clangd/issues/1892
export module generators.iterable;

import generator_v2;


#ifndef ERROR
    #define ERROR 0
#endif

namespace {
    NGeneratorV2::TGeneratorV2<int> NaturalNums(size_t end) {
        auto _ = std::suspend_never{}; // NOTE: clangd issues with unused #include <coroutine>
        for (int num = 0; num < end; num += 1) {
            co_yield num;
        }
    }
}


GTEST_TEST(cpp23_coro_generators_iterable, simple_test) {
    static constexpr size_t NUMS_COUNT = 10;
    static constexpr size_t FIRST_ROUND = 5;
#if ERROR
    {
        // NOTE: view composition requires rvalues ref (&&) or copiable objects
        auto nums = ::NaturalNums(NUMS_COUNT);
        for (const auto& value : std::views::take(nums, FIRST_ROUND)) {
            // pass
        }
    }
#endif
    {
        // the safest generator usage
        auto nums = ::NaturalNums(NUMS_COUNT) | std::views::take(FIRST_ROUND);
        for (size_t n = 0; const auto& value : nums) {
            GTEST_ASSERT_EQ(value, n++);
        }
        GTEST_ASSERT_NE(nums.begin(), nums.end());
        GTEST_ASSERT_EQ(*nums.begin(), FIRST_ROUND);
    }
    {
        // hack to a use move-only range in multiple chains
        auto nums = ::NaturalNums(NUMS_COUNT);

        for (size_t n = 0; const auto& value : std::ranges::subrange(nums) | std::views::take(FIRST_ROUND)) {
            GTEST_ASSERT_EQ(value, n++);
        }
        GTEST_ASSERT_NE(nums.begin(), nums.end());
        GTEST_ASSERT_EQ(*nums.begin(), FIRST_ROUND);

        for (size_t n = FIRST_ROUND; const auto& value : std::ranges::subrange(nums)) {
            GTEST_ASSERT_EQ(value, n++);
        }
        GTEST_ASSERT_EQ(nums.begin(), nums.end());
        GTEST_ASSERT_EQ(*nums.begin(), NUMS_COUNT);
    }
}

#undef ERROR
