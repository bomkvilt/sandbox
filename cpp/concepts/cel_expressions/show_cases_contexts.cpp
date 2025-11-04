#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// import: cel-cpp
#include "cpp/utils/contrib/cel_cpp_tweaks.hpp"
SUPPRESS_CEL_CPP_WARNINGS_BEGIN()
#include <eval/public/activation.h>
#include <eval/public/activation_bind_helper.h>
SUPPRESS_CEL_CPP_WARNINGS_END()

#include "cpp/concepts/cel_expressions/common.hpp"
#include "cpp/concepts/cel_expressions/proto/messages.pb.h"

namespace expr = ::google::api::expr;

using ::show_cases::CelExpressionsTest;

// =================================================================================================
// :: Proto Messages (contexts)
// =================================================================================================

TEST_F(CelExpressionsTest, ProtoMessageAccess) {
    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        CEL_RETURN_IF_ERROR(
            builder.AddContextDeclaration(::cel_expressions::proto::ContextA::descriptor()->full_name()) //
        );
        CEL_RETURN_IF_ERROR(
            builder.AddContextDeclaration(::cel_expressions::proto::ContextB::descriptor()->full_name()) //
        );
        return ::absl::OkStatus();
    };

    const auto context_a = [&]() -> ::cel_expressions::proto::ContextA {
        ::cel_expressions::proto::ContextA context;
        auto& message = *context.mutable_a();
        auto& sub = *message.mutable_sub_message();
        sub.set_field_1(42);
        sub.set_field_2(100);
        return context;
    }();
    const auto context_b = [&]() -> ::cel_expressions::proto::ContextB {
        ::cel_expressions::proto::ContextB context;
        auto& message = *context.mutable_b();
        auto& sub = *message.mutable_sub_message();
        sub.set_field_1(-42);
        sub.set_field_2(-100);
        return context;
    }();
    const auto activation_or = [&]() -> ::absl::StatusOr<expr::runtime::Activation> {
        expr::runtime::Activation activation;
        CEL_RETURN_IF_ERROR(
            expr::runtime::BindProtoToActivation(&context_a, &arena_, &activation) //
        );
        CEL_RETURN_IF_ERROR(
            expr::runtime::BindProtoToActivation(&context_b, &arena_, &activation) //
        );
        return activation;
    }();
    ASSERT_TRUE(activation_or.ok()) << activation_or.status().message();

    // Execute
    struct TestCase final {
        std::string_view expression;
        int32_t expected;
    };
    const std::vector<TestCase> test_cases = std::vector<TestCase>{
        {.expression = "a.sub_message.field_1 + b.sub_message.field_1", .expected = 0},
        {.expression = "a.sub_message.field_2 + b.sub_message.field_2", .expected = 0},
        {.expression = "a.sub_message.field_1 + a.sub_message.field_2", .expected = 142},
        {.expression = "b.sub_message.field_1 + b.sub_message.field_2", .expected = -142},
    };
    for (const auto& tc : test_cases) {
        const auto result = EvaluateExpression(
            tc.expression, //
            activation_or.value(),
            add_declarations
        );
        ASSERT_TRUE(result.ok()) << result.status().message() << " for expression: " << tc.expression;

        ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kInt)
            << " for expression: '" << tc.expression << "': " << result->DebugString();
        ASSERT_EQ(result->Int64OrDie(), tc.expected) << " for expression: " << tc.expression;
    }
}
