#include <any>
#include <string>

#include <gtest/gtest.h>

#include "visit.hpp"

struct A {
    int x;
};

struct B {
    std::string s;
};

// NOLINTBEGIN(readability-magic-numbers)

TEST(VisitAnyTest, VisitA) {
    std::any a = A{42};
    bool called = false;

    // clang-format off
    const auto result = visit_any::visit_any(a, visit_any::Overloads{
        [&](const A& val) -> int {
            called = true;
            return val.x;
        },
        [&](const B&) -> int {
            return -1; //
        },
        [&]() -> int {
            return 0; //
        },
    });
    // clang-format on
    EXPECT_TRUE(called);
    EXPECT_EQ(result, 42);
}

TEST(VisitAnyTest, VisitB) {
    std::any b = B{"hello"};

    // clang-format off
    const auto result = visit_any::visit_any(b, visit_any::Overloads{
        [&](const A&) -> std::string {
            return "A"; //
        },
        [&](const B& val) -> std::string {
            return val.s; //
        },
        [&]() -> std::string {
            return "default"; //
        },
    });
    // clang-format on

    EXPECT_EQ(result, "hello");
}

TEST(VisitAnyTest, VisitMutable) {
    std::any a = A{7};

    // clang-format off
    const int result = visit_any::visit_any(a, visit_any::Overloads{
        [&](A& val) -> int {
            val.x = 42;
            return 48;
        },
    });
    // clang-format on

    EXPECT_EQ(result, 48);
    EXPECT_EQ(std::any_cast<const A&>(a).x, 42);
}

TEST(VisitAnyTest, VisitDefault) {
    std::any n = 3.14F;

    // clang-format off
    auto result = visit_any::visit_any(n, visit_any::Overloads{
        [&](const A&) -> int {
            return 1; //
        },
        [&]() -> int {
            return 99; //
        },
    });
    // clang-format on

    EXPECT_EQ(result, 99);
}

TEST(VisitAnyTest, VisitThrowsOnNoMatch) {
    EXPECT_THROW(
        {
            std::any n = 3.14F;

            // clang-format off
            visit_any::visit_any(n, visit_any::Overloads{
                [&](const A&) -> int {
                    return 1; //
                },
                [&](const B&) -> int {
                    return 2; //
                },
            });
            // clang-format on
        },
        visit_any::BadAnyCast
    );
}

// -----------------------------------------------------------------------------

TEST(VisitAnyTest, VisitVoidReturn) {
    std::any a = A{7};
    int result = 0;

    // clang-format off
    visit_any::visit_any(a, visit_any::Overloads{
        [&](const A& val) -> void {
            result = val.x; //
        },
        [&](const B&) -> void {
            result = -1; //
        },
        [&]() -> void {
            result = 100; //
        },
    });
    // clang-format on

    EXPECT_EQ(result, 7);
}

TEST(VisitAnyTest, VisitMutable_VoidReturn) {
    std::any a = A{7};

    // clang-format off
    visit_any::visit_any(a, visit_any::Overloads{
        [&](A& val) -> void {
            val.x = 42; //
        },
    });
    // clang-format on

    EXPECT_EQ(std::any_cast<const A&>(a).x, 42);
}

TEST(VisitAnyTest, VisitDefault_VoidReturn) {
    std::any n = 1.23;
    int result = 0;

    // clang-format off
    visit_any::visit_any(n, visit_any::Overloads{
        [&](const A&) -> void {
            result = 1; //
        },
        [&]() -> void {
            result = 42; //
        },
    });
    // clang-format on

    EXPECT_EQ(result, 42);
}

TEST(VisitAnyTest, VisitThrowsOnNoMatch_VoidReturn) {
    EXPECT_THROW(
        {
            std::any n = 3.14F;

            // clang-format off
            visit_any::visit_any(n, visit_any::Overloads{
                [&](const A&) -> void {
                    // pass
                },
                [&](const B&) -> void {
                    // pass
                },
            });
            // clang-format on
        },
        visit_any::BadAnyCast
    );
}

// NOLINTEND(readability-magic-numbers)
