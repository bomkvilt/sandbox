#include <memory>

#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// import: cel-cpp
#include "cpp/utils/contrib/cel_cpp_tweaks.hpp"
SUPPRESS_CEL_CPP_WARNINGS_BEGIN()
#include <cel/expr/checked.pb.h>
#include <common/ast_proto.h>
#include <compiler/compiler.h>
#include <compiler/compiler_factory.h>
#include <compiler/standard_library.h>
#include <eval/public/activation.h>
#include <eval/public/builtin_func_registrar.h>
#include <eval/public/cel_expr_builder_factory.h>
#include <eval/public/cel_expression.h>
#include <eval/public/cel_function_adapter.h>
#include <eval/public/cel_options.h>
#include <eval/public/cel_value.h>
SUPPRESS_CEL_CPP_WARNINGS_END()

#include "cpp/concepts/cel_expressions/proto/messages.pb.h"
#include "cpp/utils/tests/gtest_golden/golden.hpp"

namespace show_cases {
    namespace expr = ::google::api::expr;

    struct ProtoDescriptors final {
        const ::google::protobuf::DescriptorPool& descriptor_pool;
        ::google::protobuf::MessageFactory& message_factory;

        static auto Default() -> ProtoDescriptors {
            return ProtoDescriptors{
                .descriptor_pool = *::google::protobuf::DescriptorPool::generated_pool(),
                .message_factory = *::google::protobuf::MessageFactory::generated_factory(),
            };
        };
    };

    class CelExpressionsTest : public ::testing::Test {
    protected:
        using AddVariablesFn = std::function<::absl::Status(::cel::TypeCheckerBuilder&)>;
        using AddFunctionsFn = std::function<::absl::Status(expr::runtime::CelExpressionBuilder&)>;

        ProtoDescriptors proto_ = ProtoDescriptors::Default();
        ::google::protobuf::Arena arena_;

    protected:
        ::absl::StatusOr<::cel::expr::CheckedExpr>
        MakeExpression(const std::string_view expression, const AddVariablesFn& add_declarations = nullptr) const {
            const ::cel::CompilerOptions options;

            CEL_ASSIGN_OR_RETURN(
                auto builder, //
                ::cel::NewCompilerBuilder(&proto_.descriptor_pool, options)
            );
            CEL_RETURN_IF_ERROR(
                builder->AddLibrary(::cel::StandardCompilerLibrary()) //
            );

            // Add variable descriptors
            if (add_declarations != nullptr) {
                CEL_RETURN_IF_ERROR(
                    add_declarations(builder->GetCheckerBuilder()) //
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
        ::absl::StatusOr<std::unique_ptr<expr::runtime::CelExpression>>
        MakePlan(const ::cel::expr::CheckedExpr& expr, const AddFunctionsFn& add_definitions) const {
            const expr::runtime::InterpreterOptions options;

            auto builder = expr::runtime::CreateCelExpressionBuilder(
                &proto_.descriptor_pool, //
                &proto_.message_factory,
                options
            );
            CEL_RETURN_IF_ERROR(
                expr::runtime::RegisterBuiltinFunctions(builder->GetRegistry(), options) //
            );
            if (add_definitions != nullptr) {
                CEL_RETURN_IF_ERROR(
                    add_definitions(*builder) //
                );
            }
            return builder->CreateExpression(&expr);
        };

        ::absl::StatusOr<expr::runtime::CelValue> EvaluateExpression(
            const std::string_view expression,
            const expr::runtime::Activation& activation,
            const AddVariablesFn& add_declarations = nullptr,
            const AddFunctionsFn& add_definitions = nullptr
        ) {
            CEL_ASSIGN_OR_RETURN(
                const auto expr, //
                MakeExpression(expression, add_declarations)
            );
            CEL_ASSIGN_OR_RETURN(
                const auto plan, //
                MakePlan(expr, add_definitions)
            );
            return plan->Evaluate(activation, &arena_);
        }

        void CheckGoldenStatus(const ::absl::Status& status) const {
            EXPECT_THAT(status.message(), ::golden::Golden(test_file()));
        }

    private:
        [[nodiscard]]
        std::string test_name() const {
            const char* suite = ::testing::UnitTest::GetInstance()->current_test_suite()->name();
            const char* test = ::testing::UnitTest::GetInstance()->current_test_info()->name();
            return std::format("{}.{}", suite, test);
        }

        [[nodiscard]]
        std::string test_file() const {
            constexpr std::string_view file = __FILE__;
            constexpr std::string_view base = file.substr(0U, file.find_last_of('/'));
            return std::format("{}/golden/{}", base, test_name());
        }
    };
} // namespace show_cases
