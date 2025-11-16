#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// import: cel-cpp
#include "cpp/utils/contrib/cel_cpp_tweaks.hpp"
SUPPRESS_CEL_CPP_WARNINGS_BEGIN()
#include <common/type.h>
#include <eval/public/activation.h>
#include <eval/public/cel_value.h>
SUPPRESS_CEL_CPP_WARNINGS_END()

#include "cpp/concepts/cel_expressions/common.hpp"

namespace expr = ::google::api::expr;

using ::show_cases::CelExpressionsTest;

// =================================================================================================
// :: Basic Types
// =================================================================================================

TEST_F(CelExpressionsTest, BasicArithmetic) {
    const std::string_view expression = "(10 + 5) * 2 - 3 / 1";
    const auto result = EvaluateExpression(expression, {});
    ASSERT_TRUE(result.ok()) << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kInt64);
    ASSERT_EQ(result->Int64OrDie(), 27);
}

TEST_F(CelExpressionsTest, StringOperations) {
    constexpr std::string_view expression = "'Hello' + ' ' + 'World'";
    const auto result = EvaluateExpression(expression, {});
    ASSERT_TRUE(result.ok()) << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kString);
    ASSERT_EQ(result->StringOrDie().value(), "Hello World");
}

TEST_F(CelExpressionsTest, BooleanLogic) {
    constexpr std::string_view expression = "true && false || true";
    const auto result = EvaluateExpression(expression, {});
    ASSERT_TRUE(result.ok()) << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kBool);
    ASSERT_TRUE(result->BoolOrDie());
}

// =================================================================================================
// :: Variables
// =================================================================================================

TEST_F(CelExpressionsTest, Variables) {
    const auto activation = [&]() {
        expr::runtime::Activation activation;
        activation.InsertValue("age", expr::runtime::CelValue::CreateInt64(25));
        return activation;
    }();

    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        CEL_RETURN_IF_ERROR(
            builder.AddVariable(::cel::MakeVariableDecl("age", ::cel::IntType())) //
        );
        return ::absl::OkStatus();
    };

    constexpr std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = EvaluateExpression(expression, activation, add_declarations);
    ASSERT_TRUE(result.ok()) << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kString);
    ASSERT_EQ(result->StringOrDie().value(), "adult");
}

TEST_F(CelExpressionsTest, VariablesWrongSlotType) {
    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        CEL_RETURN_IF_ERROR(
            builder.AddVariable(::cel::MakeVariableDecl("age", ::cel::StringType())) //
        );
        return ::absl::OkStatus();
    };

    constexpr std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = MakeExpression(expression, add_declarations);
    ASSERT_FALSE(result.ok());
    ASSERT_THAT(
        result.status().message(), //
        ::testing::HasSubstr("found no matching overload for '_>=_' applied to '(string, int)'")
    );
}

TEST_F(CelExpressionsTest, VariablesNoSlotSlotTypes) {
    constexpr std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = MakeExpression(expression);
    ASSERT_FALSE(result.ok());
    ASSERT_THAT(
        result.status().message(), //
        ::testing::HasSubstr("undeclared reference to 'age'")
    );
}
