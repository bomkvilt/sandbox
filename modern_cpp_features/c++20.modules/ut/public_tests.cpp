module;

#include "gtest/gtest.h"

// NOTE: the unit must be a module to avoid clangd errors like
// ```In included file: 'std::fpos' has different definitions in different modules; first difference is defined here found constructor with body```

// NOTE: The error affects clangd only; code is compilable
// SA: https://github.com/clangd/clangd/issues/1293
// SA: https://github.com/clangd/clangd/issues/1892
export module test_module.ut;

import test_module;


TEST(cpp20_modules, public_main) {
    const auto object = NTestModule::TPublicClass{};
    GTEST_ASSERT_EQ(object.Name(), "public_class");
}

TEST(cpp20_modules, public_submodule) {
    const auto object = NTestModule::TPublicClassA{};
    GTEST_ASSERT_EQ(object.Name(), "public_class_a");
}
