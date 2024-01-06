#include "gtest/gtest.h"

#include <iostream>
#include <ranges>
#include <sstream>
#include <string_view>
#include <vector>


namespace {
    static constexpr auto mt = [](std::ostream& os, std::string_view c) mutable {
        return [c, &os](auto v) { os << c << "{" << v << "}"; return v; };
    };
    static constexpr auto mf = [](std::ostream& os, std::string_view c) mutable {
        return [c, &os](auto v) { os << c << "{" << v << "}"; return 1; };
    };
}


TEST(cpp23_views, filters_transforms) {
    /**
    case study: filters and transforms
    https://stackoverflow.com/questions/69265248/is-there-an-efficient-way-to-use-viewsfilter-after-transform-range-adaptors

    std::views::filter produces O(n depth^2) calls complexity (!)

    output string (raw): T1{1}F1{1}T1{1}T2{1}F2{1}T1{1}T2{1}T3{1}F3{1}T1{1}T2{1}T3{1}T4{1}F4{1}T1{1}T2{1}T3{1}T4{1}-1
    output string: T1F1T1T2F2T1T2T3F3T1T2T3T4F4T1T2T3T4-1
    grouped string: (T1)[F1] (T1T2)[F2] (T1T2T3)[F3] (T1T2T3T4)[F4] (T1T2T3T4)-1

    Views don't cache ther results:
    F1(T1)          - F1 dereferences an input iterator (recalcs the value)
    F2(T2-T1)       - F2 dereferences an input iterator (recalcs the value)
    F3(T3-T2-T1)    - F3 dereferences an input iterator (recalcs the value)
    F4(T4-T3-T2-T1) - F4 dereferences an input iterator (recalcs the value)

    result = T4-T3-T2-T1 - last view dereference -> `auto v`
    */
    static constexpr std::string_view EXPECTED =
        "T1{1}F1{1}T1{1}T2{1}F2{1}T1{1}T2{1}T3{1}F3{1}"
        "T1{1}T2{1}T3{1}T4{1}F4{1}T1{1}T2{1}T3{1}T4{1}-1"
        "T1{2}F1{2}T1{2}T2{2}F2{2}T1{2}T2{2}T3{2}F3{2}"
        "T1{2}T2{2}T3{2}T4{2}F4{2}T1{2}T2{2}T3{2}T4{2}-2"
        "T1{3}F1{3}T1{3}T2{3}F2{3}T1{3}T2{3}T3{3}F3{3}"
        "T1{3}T2{3}T3{3}T4{3}F4{3}T1{3}T2{3}T3{3}T4{3}-3"
    ;

    std::stringstream ss {};
    std::vector<int> vec = {1, 2, 3};
    auto view = vec
        | std::views::transform(::mt(ss, "T1"))
        | std::views::filter(::mf(ss, "F1"))
        | std::views::transform(::mt(ss, "T2"))
        | std::views::filter(::mf(ss, "F2"))
        | std::views::transform(::mt(ss, "T3"))
        | std::views::filter(::mf(ss, "F3"))
        | std::views::transform(::mt(ss, "T4"))
        | std::views::filter(::mf(ss, "F4"))
    ;
    for(auto v : view) {
        ss << "-" << v;
    }

    std::cerr << ss.view() << std::endl;
    GTEST_ASSERT_EQ(ss.view(), EXPECTED);
}


GTEST_TEST(cpp23_views, filters_transforms_to) {
    /**
    case study: filters, transforms, to;

    std::views::filter produces O(n depth^2) calls complexity (!)
    std::views::filter produces additional O(n d) calls complexity with std::ranges::to (!)

    output string: T1{1}F1{1}T1{1}T2{1}F2{1}T1{1}T2{1}T3{1}F3{1}T1{1}T2{1}T3{1}T4{1}F4{1}T1{2}F1{2}T1{2}T2{2}F2{2}T1{2}T2{2}T3{2}F3{2}T1{2}T2{2}T3{2}T4{2}F4{2}T1{3}F1{3}T1{3}T2{3}F2{3}T1{3}T2{3}T3{3}F3{3}T1{3}T2{3}T3{3}T4{3}F4{3}T1{1}T2{1}T3{1}T4{1}T1{2}F1{2}T1{2}T2{2}F2{2}T1{2}T2{2}T3{2}F3{2}T1{2}T2{2}T3{2}T4{2}F4{2}T1{2}T2{2}T3{2}T4{2}T1{3}F1{3}T1{3}T2{3}F2{3}T1{3}T2{3}T3{3}F3{3}T1{3}T2{3}T3{3}T4{3}F4{3}T1{3}T2{3}T3{3}T4{3}-1-2-3
    grouped string:
        # first access to {1}:
        T1{1}F1{1}
        T1{1}T2{1}F2{1}
        T1{1}T2{1}T3{1}F3{1}
        T1{1}T2{1}T3{1}T4{1}F4{1}

        # first access to {2}:
        T1{2}F1{2}
        T1{2}T2{2}F2{2}
        T1{2}T2{2}T3{2}F3{2}
        T1{2}T2{2}T3{2}T4{2}F4{2}

        # first access to {3}:
        T1{3}F1{3}
        T1{3}T2{3}F2{3}
        T1{3}T2{3}T3{3}F3{3}
        T1{3}T2{3}T3{3}T4{3}F4{3}

        # consumption of {1}:
        T1{1}T2{1}T3{1}T4{1}

        # secondary access to {2}: (???)
        T1{2}F1{2}
        T1{2}T2{2}F2{2}
        T1{2}T2{2}T3{2}F3{2}
        T1{2}T2{2}T3{2}T4{2}F4{2}

        # consumption of {2}:
        T1{2}T2{2}T3{2}T4{2}

        # secondary access to {3}: (???)
        T1{3}F1{3}
        T1{3}T2{3}F2{3}
        T1{3}T2{3}T3{3}F3{3}
        T1{3}T2{3}T3{3}T4{3}F4{3}

        # consumption of {3}:
        T1{3}T2{3}T3{3}T4{3}
    */
    static constexpr std::string_view EXPECTED =
        "T1{1}F1{1}T1{1}T2{1}F2{1}T1{1}T2{1}T3{1}F3{1}T1{1}T2{1}T3{1}T4{1}F4{1}"
        "T1{2}F1{2}T1{2}T2{2}F2{2}T1{2}T2{2}T3{2}F3{2}T1{2}T2{2}T3{2}T4{2}F4{2}"
        "T1{3}F1{3}T1{3}T2{3}F2{3}T1{3}T2{3}T3{3}F3{3}T1{3}T2{3}T3{3}T4{3}F4{3}"
        "T1{1}T2{1}T3{1}T4{1}"
        "T1{2}F1{2}T1{2}T2{2}F2{2}T1{2}T2{2}T3{2}F3{2}T1{2}T2{2}T3{2}T4{2}F4{2}"
        "T1{2}T2{2}T3{2}T4{2}"
        "T1{3}F1{3}T1{3}T2{3}F2{3}T1{3}T2{3}T3{3}F3{3}T1{3}T2{3}T3{3}T4{3}F4{3}"
        "T1{3}T2{3}T3{3}T4{3}"
        "-1-2-3"
    ;

    std::stringstream ss {};
    std::vector<int> vec = {1, 2, 3};
    auto elems = vec
        | std::views::transform(::mt(ss, "T1"))
        | std::views::filter(::mf(ss, "F1"))
        | std::views::transform(::mt(ss, "T2"))
        | std::views::filter(::mf(ss, "F2"))
        | std::views::transform(::mt(ss, "T3"))
        | std::views::filter(::mf(ss, "F3"))
        | std::views::transform(::mt(ss, "T4"))
        | std::views::filter(::mf(ss, "F4"))
        | std::ranges::to<std::vector>()
    ;
    for(auto v : elems) {
        ss << "-" << v;
    }

    std::cerr << ss.view() << std::endl;
    GTEST_ASSERT_EQ(ss.view(), EXPECTED);
}


GTEST_TEST(cpp23_views, transforms_to) {
    /**
    case study: transforms, to

    output string: T1{1}T2{1}T3{1}T4{1}T1{2}T2{2}T3{2}T4{2}T1{3}T2{3}T3{3}T4{3}-1-2-3
    grouped string:
        # single access to {1}:
        "T1{1}T2{1}T3{1}T4{1}"

        # single access to {2}:
        "T1{2}T2{2}T3{2}T4{2}"

        # single access to {3}:
        "T1{3}T2{3}T3{3}T4{3}"
    */
    static constexpr std::string_view EXPECTED =
        "T1{1}T2{1}T3{1}T4{1}"
        "T1{2}T2{2}T3{2}T4{2}"
        "T1{3}T2{3}T3{3}T4{3}"
        "-1-2-3"
    ;

    std::stringstream ss {};
    std::vector<int> vec = {1, 2, 3};
    auto elems = vec
        | std::views::transform(::mt(ss, "T1"))
        | std::views::transform(::mt(ss, "T2"))
        | std::views::transform(::mt(ss, "T3"))
        | std::views::transform(::mt(ss, "T4"))
        | std::ranges::to<std::vector>()
    ;
    for(auto v : elems) {
        ss << "-" << v;
    }

    std::cerr << ss.view() << std::endl;
    GTEST_ASSERT_EQ(ss.view(), EXPECTED);
}
