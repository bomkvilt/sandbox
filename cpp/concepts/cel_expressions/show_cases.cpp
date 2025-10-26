#include <memory>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

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
#include "eval/public/builtin_func_registrar.h"
#include "eval/public/cel_expr_builder_factory.h"
#include "eval/public/cel_expression.h"
#include "eval/public/cel_options.h"
#include "eval/public/cel_value.h"
SUPPRESS_CEL_CPP_WARNINGS_END()

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
        ProtoPoolDescriptors proto_ = ProtoPoolDescriptors::Default();
        ::google::protobuf::Arena arena_;

    protected:
        ::absl::StatusOr<::cel::expr::CheckedExpr> MakeExpression(
            const ::std::string_view expression,
            const std::span<const ::cel::VariableDecl> variables
        ) const {
            const ::cel::CompilerOptions options;

            CEL_ASSIGN_OR_RETURN(
                auto builder, //
                ::cel::NewCompilerBuilder(&proto_.descriptor_pool, options)
            );
            CEL_RETURN_IF_ERROR(
                builder->AddLibrary(::cel::StandardCompilerLibrary()) //
            );

            // Add variable descriptors
            for (const auto& variable : variables) {
                CEL_RETURN_IF_ERROR(builder->GetCheckerBuilder().AddVariable(variable));
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
            const std::span<const ::cel::VariableDecl> variables,
            const expr::runtime::Activation& activation
        ) {
            CEL_ASSIGN_OR_RETURN(
                const auto expr, //
                MakeExpression(expression, variables)
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

TEST_F(CelExpressionsTest, BasicArithmetic) {
    const ::std::string_view expression = "(10 + 5) * 2 - 3 / 1";
    const auto result = EvaluateExpression(expression, {}, {});
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kInt64);
    ASSERT_EQ(result->Int64OrDie(), 27);
}

TEST_F(CelExpressionsTest, StringOperations) {
    constexpr ::std::string_view expression = "'Hello' + ' ' + 'World'";
    const auto result = EvaluateExpression(expression, {}, {});
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kString);
    ASSERT_EQ(result->StringOrDie().value(), "Hello World");
}

TEST_F(CelExpressionsTest, BooleanLogic) {
    constexpr ::std::string_view expression = "true && false || true";
    const auto result = EvaluateExpression(expression, {}, {});
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kBool);
    ASSERT_TRUE(result->BoolOrDie());
}

TEST_F(CelExpressionsTest, Variables) {
    const ::expr::runtime::Activation activation = [&]() {
        ::expr::runtime::Activation activation;
        activation.InsertValue("age", expr::runtime::CelValue::CreateInt64(25));
        return activation;
    }();
    const ::std::vector<::cel::VariableDecl> variables{
        ::cel::MakeVariableDecl("age", ::cel::IntType()),
    };

    constexpr ::std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = EvaluateExpression(expression, variables, activation);
    ASSERT_TRUE(result.ok()) << FTEE << result.status().message();

    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kString);
    ASSERT_EQ(result->StringOrDie().value(), "adult");
}

TEST_F(CelExpressionsTest, VariablesWrongSlotType) {
    const ::std::vector<::cel::VariableDecl> variables{
        ::cel::MakeVariableDecl("age", ::cel::StringType()), // NOTE: int -> string
    };

    constexpr ::std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = MakeExpression(expression, variables);
    ASSERT_FALSE(result.ok());
    ASSERT_THAT(
        result.status().message(), //
        ::testing::HasSubstr("found no matching overload for '_>=_' applied to '(string, int)'")
    );
}

TEST_F(CelExpressionsTest, VariablesNoSlotSlotTypes) {
    constexpr ::std::string_view expression = "age >= 18 ? 'adult' : 'minor'";
    const auto result = MakeExpression(expression, {});
    ASSERT_FALSE(result.ok());
    ASSERT_THAT(
        result.status().message(), //
        ::testing::HasSubstr("undeclared reference to 'age'")
    );
}
