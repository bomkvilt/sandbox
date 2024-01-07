module;

#include "gtest/gtest.h"

// NOTE: the .cpp file is located in other cmake target but uses the same module name to access one's :private fragment.
// IMHO, unit tests must use the same module name as the being tested module.
module test_module;

TEST(cpp20_modules, private_main) {
    const auto object = NTestModule::TPrivateClass{};
    GTEST_ASSERT_EQ(object.Name(), "private_class");
}
