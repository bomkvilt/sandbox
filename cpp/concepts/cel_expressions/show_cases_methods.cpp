#include <cstdint>

#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// import: cel-cpp
#include "cpp/utils/contrib/cel_cpp_tweaks.hpp"
SUPPRESS_CEL_CPP_WARNINGS_BEGIN()
#include <common/type.h>
#include <eval/public/activation.h>
#include <eval/public/activation_bind_helper.h>
#include <eval/public/cel_expression.h>
#include <eval/public/cel_value.h>
SUPPRESS_CEL_CPP_WARNINGS_END()

#include "cpp/concepts/cel_expressions/common.hpp"
#include "cpp/concepts/cel_expressions/proto/messages.pb.h"

namespace expr = ::google::api::expr;

using ::show_cases::CelExpressionsTest;

// =================================================================================================
// :: Methods
// =================================================================================================

namespace {
    struct CustomContainsKey final {
        static ::absl::Status AddDeclarations(::cel::TypeCheckerBuilder& builder) {
            const auto key_type = cel::StringType();
            const auto generic_value_type = cel::TypeParamType("V");
            const auto map_type = cel::MapType(builder.arena(), key_type, generic_value_type);

            CEL_ASSIGN_OR_RETURN(
                cel::FunctionDecl decl,
                cel::MakeFunctionDecl(
                    "custom_contains",
                    cel::MakeMemberOverloadDecl( // NOTE: promise: receiver_style=true
                        "custom_map_contains_k_string",
                        /*result=*/cel::BoolType(),
                        map_type,
                        key_type
                    )
                )
            );
            CEL_RETURN_IF_ERROR(
                builder.MergeFunction(decl) // NOTE: overloads -> merge
            );
            return ::absl::OkStatus();
        }

        static ::absl::Status AddDefinitions(::expr::runtime::CelExpressionBuilder& builder) {
            using Adapter = expr::runtime::FunctionAdapter<
                ::absl::StatusOr<bool>,
                const expr::runtime::CelMap*,
                expr::runtime::CelValue::StringHolder
            >;
            CEL_RETURN_IF_ERROR(
                Adapter::CreateAndRegister(
                    "custom_contains",
                    /*receiver_style=*/true,
                    HandlerKey,
                    builder.GetRegistry()
                )
            );
            return ::absl::OkStatus();
        }

    private:
        static ::absl::StatusOr<bool> HandlerKey(
            google::protobuf::Arena* arena,
            const expr::runtime::CelMap* map,
            const expr::runtime::CelValue::StringHolder key
        ) {
            const auto entry = map->Get(arena, expr::runtime::CelValue::CreateString(key));
            return entry.has_value();
        }
    };

    struct CustomContainsKeyValue final {
        static ::absl::Status AddDeclarations(::cel::TypeCheckerBuilder& builder) {
            const auto key_type = cel::StringType();
            const auto generic_value_type = cel::TypeParamType("V");
            const auto map_type = cel::MapType(builder.arena(), key_type, generic_value_type);

            CEL_ASSIGN_OR_RETURN(
                cel::FunctionDecl decl,
                cel::MakeFunctionDecl(
                    "custom_contains",
                    cel::MakeMemberOverloadDecl( // NOTE: promise: receiver_style=true
                        "custom_map_contains_kv_string_any",
                        /*result=*/cel::BoolType(),
                        map_type,
                        key_type,
                        generic_value_type
                    )
                )
            );
            CEL_RETURN_IF_ERROR(
                builder.MergeFunction(decl) // NOTE: overloads -> merge
            );
            return ::absl::OkStatus();
        }

        static ::absl::Status AddDefinitions(::expr::runtime::CelExpressionBuilder& builder) {
            using Adapter = expr::runtime::FunctionAdapter<
                ::absl::StatusOr<bool>,
                const expr::runtime::CelMap*,
                expr::runtime::CelValue::StringHolder,
                expr::runtime::CelValue
            >;
            CEL_RETURN_IF_ERROR(
                Adapter::CreateAndRegister(
                    "custom_contains",
                    /*receiver_style=*/true,
                    HandlerKeyValue,
                    builder.GetRegistry()
                )
            );
            return ::absl::OkStatus();
        }

    private:
        static ::absl::StatusOr<bool> HandlerKeyValue(
            google::protobuf::Arena* arena,
            const expr::runtime::CelMap* map,
            const expr::runtime::CelValue::StringHolder key,
            const expr::runtime::CelValue& value
        ) {
            const auto entry = map->Get(arena, expr::runtime::CelValue::CreateString(key));
            if (!entry.has_value()) {
                return false;
            }
            if (value.IsInt64() && entry->IsInt64()) {
                return value.Int64OrDie() == entry->Int64OrDie();
            }
            if (value.IsString() && entry->IsString()) {
                return value.StringOrDie().value() == entry->StringOrDie().value();
            }
            return false;
        }
    };
} // namespace

TEST_F(CelExpressionsTest, MethodCall) {
    // Stage: declarations
    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        // Add context declarations
        CEL_RETURN_IF_ERROR(
            builder.AddContextDeclaration(::cel_expressions::proto::ContextC::descriptor()->full_name()) //
        );
        // Add function declarations
        CEL_RETURN_IF_ERROR(
            CustomContainsKey::AddDeclarations(builder) //
        );
        CEL_RETURN_IF_ERROR(
            CustomContainsKeyValue::AddDeclarations(builder) //
        );
        return ::absl::OkStatus();
    };

    // Stage: definitions
    const auto add_definitions = [&](::expr::runtime::CelExpressionBuilder& builder) -> ::absl::Status {
        CEL_RETURN_IF_ERROR(
            CustomContainsKey::AddDefinitions(builder) //
        );
        CEL_RETURN_IF_ERROR(
            CustomContainsKeyValue::AddDefinitions(builder) //
        );
        return ::absl::OkStatus();
    };

    // Stage: context preparation
    const auto context_c = [&]() -> ::cel_expressions::proto::ContextC {
        ::cel_expressions::proto::ContextC context;
        context.mutable_map_field()->insert({"key-1", 1});
        context.mutable_map_field()->insert({"key-2", 2});
        return context;
    }();
    const auto activation_or = [&]() -> ::absl::StatusOr<expr::runtime::Activation> {
        expr::runtime::Activation activation;
        CEL_RETURN_IF_ERROR(
            expr::runtime::BindProtoToActivation(&context_c, &arena_, &activation) //
        );
        return activation;
    }();
    ASSERT_TRUE(activation_or.ok()) << activation_or.status().message();

    // Stage: execution
    enum class ExpectedResult : int8_t {
        kTrue,
        kFalse,
        kFailure,
    };
    struct TestCase final {
        std::string_view expression;
        ExpectedResult expected;
    };
    const std::vector<TestCase> test_cases = std::vector<TestCase>{
        // key and value
        {.expression = "map_field.custom_contains('key-1', 1)", .expected = ExpectedResult::kTrue},
        {.expression = "map_field.custom_contains('key-2', 2)", .expected = ExpectedResult::kTrue},
        {.expression = "map_field.custom_contains('key-1', 2)", .expected = ExpectedResult::kFalse},
        {.expression = "map_field.custom_contains('key-2', 1)", .expected = ExpectedResult::kFalse},
        {.expression = "map_field.custom_contains('key-3', 1)", .expected = ExpectedResult::kFalse},
        // wrong value type
        {.expression = "map_field.custom_contains('key-3', '1')", .expected = ExpectedResult::kFailure},
        // key only
        {.expression = "map_field.custom_contains('key-1')", .expected = ExpectedResult::kTrue},
        {.expression = "map_field.custom_contains('key-2')", .expected = ExpectedResult::kTrue},
        {.expression = "map_field.custom_contains('key-3')", .expected = ExpectedResult::kFalse},
    };
    for (const auto& tc : test_cases) {
        const auto result = EvaluateExpression(
            tc.expression, //
            activation_or.value(),
            add_declarations,
            add_definitions
        );
        if (tc.expected == ExpectedResult::kFailure) {
            ASSERT_FALSE(result.ok()) << result.status().message();
            continue;
        }

        const auto expected = tc.expected == ExpectedResult::kTrue;

        ASSERT_TRUE(result.ok()) << result.status().message();
        ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kBool) << ": " << result->DebugString();
        ASSERT_EQ(result->BoolOrDie(), expected) << ": " << result->DebugString();
    }
}
