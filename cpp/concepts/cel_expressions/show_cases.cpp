#include <memory>

#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// import: cel-cpp
#include "cpp/utils/contrib/cel_cpp_tweaks.hpp"
SUPPRESS_CEL_CPP_WARNINGS_BEGIN()
#include "cel/expr/checked.pb.h"
#include "common/ast_proto.h"
#include "common/type.h"
#include "common/types/int_type.h"
#include "compiler/compiler.h"
#include "compiler/compiler_factory.h"
#include "compiler/standard_library.h"
#include "eval/public/activation.h"
#include "eval/public/activation_bind_helper.h"
#include "eval/public/builtin_func_registrar.h"
#include "eval/public/cel_expr_builder_factory.h"
#include "eval/public/cel_expression.h"
#include "eval/public/cel_options.h"
#include "eval/public/cel_value.h"
SUPPRESS_CEL_CPP_WARNINGS_END()

#include "cpp/concepts/cel_expressions/proto/messages.pb.h"

namespace expr = ::google::api::expr;

namespace {
    constexpr ::std::string_view FTEE = "Failed to evaluate expression: ";

    struct ProtoPoolDescriptors final {
        const ::google::protobuf::DescriptorPool& descriptor_pool;
        ::google::protobuf::MessageFactory& message_factory;

        static auto Default() -> ProtoPoolDescriptors {
            return ProtoPoolDescriptors{
                .descriptor_pool = *::google::protobuf::DescriptorPool::generated_pool(),
                .message_factory = *::google::protobuf::MessageFactory::generated_factory(),
            };
        };
    };

    class CelExpressionsTest : public ::testing::Test {
    protected:
        using AddVariablesFn = ::std::function<::absl::Status(::cel::TypeCheckerBuilder&)>;

        ProtoPoolDescriptors proto_ = ProtoPoolDescriptors::Default();
        ::google::protobuf::Arena arena_;

    protected:
        ::absl::StatusOr<::cel::expr::CheckedExpr>
        MakeExpression(const ::std::string_view expression, const AddVariablesFn& add_variables = nullptr) const {
            const ::cel::CompilerOptions options;

            CEL_ASSIGN_OR_RETURN(
                auto builder, //
                ::cel::NewCompilerBuilder(&proto_.descriptor_pool, options)
            );
            CEL_RETURN_IF_ERROR(
                builder->AddLibrary(::cel::StandardCompilerLibrary()) //
            );

            // Add variable descriptors
            if (add_variables != nullptr) {
                CEL_RETURN_IF_ERROR(
                    add_variables(builder->GetCheckerBuilder()) //
                );
            }
            CEL_ASSIGN_OR_RETURN(
                const auto compiler, //
                builder->Build()
            );

            CEL_ASSIGN_OR_RETURN(
                const auto result, //
                compiler->Compile(expression)
            );
            if (!result.IsValid() || result.GetAst() == nullptr) {
                return ::absl::InvalidArgumentError(result.FormatError());
            }

            ::cel::expr::CheckedExpr checked_expr;
            CEL_RETURN_IF_ERROR(
                ::cel::AstToCheckedExpr(*result.GetAst(), &checked_expr) //
            );
            return checked_expr;
        }

        // NOTE: expression_plan is reusable and can be cached
        ::absl::StatusOr<::std::unique_ptr<expr::runtime::CelExpression>> MakePlan(
            const ::cel::expr::CheckedExpr& expr
        ) const {
            const expr::runtime::InterpreterOptions options;

            auto builder = expr::runtime::CreateCelExpressionBuilder(
                &proto_.descriptor_pool, //
                &proto_.message_factory,
                options
            );
            CEL_RETURN_IF_ERROR(
                expr::runtime::RegisterBuiltinFunctions(builder->GetRegistry(), options) //
            );
            return builder->CreateExpression(&expr);
        };

        ::absl::StatusOr<expr::runtime::CelValue> EvaluateExpression(
            const ::std::string_view expression,
            const expr::runtime::Activation& activation,
            const AddVariablesFn& add_variables = nullptr
        ) {
            CEL_ASSIGN_OR_RETURN(
                const auto expr, //
                MakeExpression(expression, add_variables)
            );
            CEL_ASSIGN_OR_RETURN(
                const auto plan, //
                MakePlan(expr)
            );
            return plan->Evaluate(activation, &arena_);
        }
    };
} // namespace

// =================================================================================================
// :: Basic Types
// =================================================================================================

TEST_F(CelExpressionsTest, BasicArithmetic) {
    const ::std::string_view expression = "(10 + 5) * 2 - 3 / 1";
    const auto result = EvaluateExpression(expression, {});
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kInt64);
    ASSERT_EQ(result->Int64OrDie(), 27);
}

TEST_F(CelExpressionsTest, StringOperations) {
    constexpr ::std::string_view expression = "'Hello' + ' ' + 'World'";
    const auto result = EvaluateExpression(expression, {});
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kString);
    ASSERT_EQ(result->StringOrDie().value(), "Hello World");
}

TEST_F(CelExpressionsTest, BooleanLogic) {
    constexpr ::std::string_view expression = "true && false || true";
    const auto result = EvaluateExpression(expression, {});
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

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

    const auto add_variables = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        CEL_RETURN_IF_ERROR(
            builder.AddVariable(::cel::MakeVariableDecl("age", ::cel::IntType())) //
        );
        return ::absl::OkStatus();
    };

    constexpr ::std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = EvaluateExpression(expression, activation, add_variables);
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kString);
    ASSERT_EQ(result->StringOrDie().value(), "adult");
}

TEST_F(CelExpressionsTest, VariablesWrongSlotType) {
    const auto add_variables = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        CEL_RETURN_IF_ERROR(
            builder.AddVariable(::cel::MakeVariableDecl("age", ::cel::StringType())) //
        );
        return ::absl::OkStatus();
    };

    constexpr ::std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = MakeExpression(expression, add_variables);
    ASSERT_FALSE(result.ok());
    ASSERT_THAT(
        result.status().message(), //
        ::testing::HasSubstr("found no matching overload for '_>=_' applied to '(string, int)'")
    );
}

TEST_F(CelExpressionsTest, VariablesNoSlotSlotTypes) {
    constexpr ::std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = MakeExpression(expression);
    ASSERT_FALSE(result.ok());
    ASSERT_THAT(
        result.status().message(), //
        ::testing::HasSubstr("undeclared reference to 'age'")
    );
}

// =================================================================================================
// :: Proto Messages (contexts)
// =================================================================================================

TEST_F(CelExpressionsTest, ProtoMessageAccess) {
    const auto add_variables = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
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
    ASSERT_TRUE(activation_or.ok()) << FTEE << activation_or.status().message();

    // Execute
    struct TestCase final {
        ::std::string_view expression;
        int32_t expected;
    };
    const std::vector<TestCase> test_cases = std::vector<TestCase>{
        {.expression = "a.sub_message.field_1 + b.sub_message.field_1", .expected = 0},
        {.expression = "a.sub_message.field_2 + b.sub_message.field_2", .expected = 0},
        {.expression = "a.sub_message.field_1 + a.sub_message.field_2", .expected = 142},
        {.expression = "b.sub_message.field_1 + b.sub_message.field_2", .expected = -142},
    };
    for (const auto& test_case : test_cases) {
        const auto result = EvaluateExpression(test_case.expression, activation_or.value(), add_variables);
        ASSERT_TRUE(result.ok()) << FTEE << result.status().message() << " for expression: " << test_case.expression;

        ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kInt)
            << " for expression: '" << test_case.expression << "': " << result->DebugString();
        ASSERT_EQ(result->Int64OrDie(), test_case.expected) << " for expression: " << test_case.expression;
    }
}

// =================================================================================================
// :: Functions
// =================================================================================================
