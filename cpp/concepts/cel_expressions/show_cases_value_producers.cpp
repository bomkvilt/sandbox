#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// import: cel-cpp
#include "cpp/utils/contrib/cel_cpp_tweaks.hpp"
SUPPRESS_CEL_CPP_WARNINGS_BEGIN()
#include <common/native_type.h>
#include <common/type.h>
#include <common/value.h>
#include <common/values/opaque_value.h>
#include <eval/public/activation.h>
#include <eval/public/activation_bind_helper.h>
#include <eval/public/cel_expression.h>
#include <eval/public/cel_value.h>
#include <eval/public/cel_value_producer.h>
#include <eval/public/structs/cel_proto_wrapper.h>
SUPPRESS_CEL_CPP_WARNINGS_END()

#include "cpp/concepts/cel_expressions/common.hpp"
#include "cpp/concepts/cel_expressions/proto/messages.pb.h"

namespace expr = ::google::api::expr;

using ::show_cases::CelExpressionsTest;

// =================================================================================================
// :: Value Providers
// =================================================================================================

namespace {
    class SimpleIntValueProducer final : public expr::runtime::CelValueProducer {
    public:
        explicit SimpleIntValueProducer(int64_t value)
            : value_(value) {
        }

        expr::runtime::CelValue Produce(google::protobuf::Arena* arena) override {
            return expr::runtime::CelValue::CreateInt64(value_);
        }

    private:
        int64_t value_;
    };

    // =================================

    class MapValueProducer final : public expr::runtime::CelValueProducer {
    public:
        explicit MapValueProducer(std::unordered_map<std::string, int64_t> values)
            : values_(std::move(values)) {
        }

        [[nodiscard]]
        expr::runtime::CelValue Produce(google::protobuf::Arena* arena) override {
            // For simplicity, return the first value in the map
            // In a real scenario, you'd use the identifier somehow
            // Note: Value producers are bound to specific identifiers via InsertValueProducer
            if (!values_.empty()) {
                return expr::runtime::CelValue::CreateInt64(values_.begin()->second);
            }
            return expr::runtime::CelValue::CreateNull();
        }

        [[nodiscard]]
        int64_t GetValue(const std::string& key) const {
            const auto it = values_.find(key);
            if (it != values_.end()) {
                return it->second;
            }
            return 0;
        }

    private:
        std::unordered_map<std::string, int64_t> values_;
    };

    // =================================

    class StringValueProducer final : public expr::runtime::CelValueProducer {
    public:
        explicit StringValueProducer(std::string value)
            : value_(std::move(value)) {
        }

        [[nodiscard]]
        expr::runtime::CelValue Produce(google::protobuf::Arena* arena) override {
            // String must be allocated on arena for non-primitive types
            auto* arena_string = google::protobuf::Arena::Create<std::string>(arena, value_);
            return expr::runtime::CelValue::CreateString(arena_string);
        }

    private:
        std::string value_;
    };

    class CallbackValueProducer final : public expr::runtime::CelValueProducer {
    public:
        explicit CallbackValueProducer(std::function<int64_t()> callback)
            : callback_(std::move(callback)) {
        }

        [[nodiscard]]
        expr::runtime::CelValue Produce(google::protobuf::Arena* arena) override {
            return expr::runtime::CelValue::CreateInt64(callback_());
        }

    private:
        std::function<int64_t()> callback_;
    };

    // =================================

    // NOTE: values are pre-defined with their types, instantiating values or producers is activation responsibility.

    struct ValueProducersSetup final {
        static ::absl::Status AddDeclarations(::cel::TypeCheckerBuilder& builder) {
            CEL_RETURN_IF_ERROR( //
                builder.AddVariable(::cel::MakeVariableDecl("my_value", ::cel::IntType()))
            );
            CEL_RETURN_IF_ERROR( //
                builder.AddVariable(::cel::MakeVariableDecl("config_value", ::cel::IntType()))
            );
            CEL_RETURN_IF_ERROR( //
                builder.AddVariable(::cel::MakeVariableDecl("dynamic_value", ::cel::IntType()))
            );
            CEL_RETURN_IF_ERROR( //
                builder.AddVariable(::cel::MakeVariableDecl("app_name", ::cel::StringType()))
            );
            return ::absl::OkStatus();
        }

        static ::absl::Status AddDefinitions(::expr::runtime::CelExpressionBuilder& builder) {
            return ::absl::OkStatus();
        }
    };
} // namespace

// =================================================================================================

TEST_F(CelExpressionsTest, ValueProducerSimple) {
    // Stage: declarations
    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        return ValueProducersSetup::AddDeclarations(builder);
    };

    // Stage: definitions
    const auto add_definitions = [&](::expr::runtime::CelExpressionBuilder& builder) -> ::absl::Status {
        return ValueProducersSetup::AddDefinitions(builder);
    };

    // Stage: activation preparation
    const auto activation = [&]() -> expr::runtime::Activation {
        expr::runtime::Activation activation;
        activation.InsertValueProducer("my_value", std::make_unique<SimpleIntValueProducer>(42));
        return activation;
    }();

    // Stage: execution
    constexpr std::string_view expression = "my_value + 10";
    const auto result = EvaluateExpression(expression, activation, add_declarations, add_definitions);

    ASSERT_TRUE(result.ok()) << result.status().message();
    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kInt64);
    ASSERT_EQ(result->Int64OrDie(), 52); // 42 + 10
}

TEST_F(CelExpressionsTest, ValueProducerMultiple) {
    // Stage: declarations
    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        return ValueProducersSetup::AddDeclarations(builder);
    };

    // Stage: definitions
    const auto add_definitions = [&](::expr::runtime::CelExpressionBuilder& builder) -> ::absl::Status {
        return ValueProducersSetup::AddDefinitions(builder);
    };

    // Stage: activation preparation
    const auto activation = [&]() -> expr::runtime::Activation {
        expr::runtime::Activation activation;
        activation.InsertValueProducer("config_value", std::make_unique<SimpleIntValueProducer>(100));
        activation.InsertValueProducer("app_name", std::make_unique<StringValueProducer>("MyApp"));
        return activation;
    }();

    // Stage: execution
    struct TestCase final {
        std::string_view expression;
        expr::runtime::CelValue::Type expected_type;
        std::variant<int64_t, std::string> expected_value;
    };
    const std::vector<TestCase> test_cases = {
        {
            .expression = "config_value * 2",
            .expected_type = expr::runtime::CelValue::Type::kInt64,
            .expected_value = int64_t{200},
        },
        {
            .expression = "app_name == 'MyApp'",
            .expected_type = expr::runtime::CelValue::Type::kBool,
            .expected_value = int64_t{1}, // true
        },
        {
            .expression = "config_value + 50",
            .expected_type = expr::runtime::CelValue::Type::kInt64,
            .expected_value = int64_t{150},
        },
    };

    for (const auto& tc : test_cases) {
        const auto result = EvaluateExpression(
            tc.expression, //
            activation,
            add_declarations,
            add_definitions
        );

        ASSERT_TRUE(result.ok()) //
            << result.status().message() << " for expression: " << tc.expression;
        ASSERT_EQ(result->type(), tc.expected_type) //
            << " for expression: " << tc.expression;

        if (tc.expected_type == expr::runtime::CelValue::Type::kInt64) {
            const auto expected = std::get<int64_t>(tc.expected_value);
            ASSERT_EQ(result->Int64OrDie(), expected) //
                << " for expression: " << tc.expression;
        } else if (tc.expected_type == expr::runtime::CelValue::Type::kBool) {
            const auto expected_bool = std::get<int64_t>(tc.expected_value) != 0;
            ASSERT_EQ(result->BoolOrDie(), expected_bool) //
                << " for expression: " << tc.expression;
        }
    }
}

TEST_F(CelExpressionsTest, ValueProducerCallback) {
    // Stage: declarations
    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        return ValueProducersSetup::AddDeclarations(builder);
    };

    // Stage: definitions
    const auto add_definitions = [&](::expr::runtime::CelExpressionBuilder& builder) -> ::absl::Status {
        return ValueProducersSetup::AddDefinitions(builder);
    };

    // Stage: activation preparation
    const auto activation = [&]() -> expr::runtime::Activation {
        expr::runtime::Activation activation;
        activation.InsertValueProducer(
            "dynamic_value",
            std::make_unique<CallbackValueProducer>([call_count = 0]() mutable -> int64_t { //
                return 10 * (++call_count);
            })
        );
        return activation;
    }();

    // Stage: execution
    constexpr std::string_view expression = "dynamic_value + dynamic_value";
    const auto result = EvaluateExpression(expression, activation, add_declarations, add_definitions);

    ASSERT_TRUE(result.ok()) << result.status().message();
    ASSERT_EQ(result->type(), expr::runtime::CelValue::Type::kInt64);
    ASSERT_EQ(result->Int64OrDie(), 20); // 10 + 10
}

// =================================================================================================

namespace {
    const expr::runtime::CelError NotImplementedError = ::absl::InvalidArgumentError("Not implemented");

    /// @brief Auxiliary class made for lazy collection key listing purposes.
    struct LazyCollectionKeyViews final : expr::runtime::CelList {
    public:
        std::vector<std::string_view> keys;

    public:
        [[nodiscard]]
        expr::runtime::CelValue operator[](int index) const override {
            return expr::runtime::CelValue::CreateError(&NotImplementedError);
        }

        [[nodiscard]]
        expr::runtime::CelValue Get(google::protobuf::Arena* arena, int index) const override {
            return expr::runtime::CelValue::CreateStringView(keys[index]);
        }

        [[nodiscard]]
        int size() const override {
            return static_cast<int>(keys.size());
        }
    };

    /// @brief A minimal lazy collection that computes values per key on first access.
    class LazyCollection final : public expr::runtime::CelMap {
    public:
        [[nodiscard]]
        ::absl::Status Set(std::string key, std::unique_ptr<expr::runtime::CelValueProducer> producer) {
            if (producer == nullptr) {
                return ::absl::InvalidArgumentError("Producer cannot be nullptr");
            }

            const auto& [it, ok] = producers_.emplace(std::move(key), std::move(producer));
            if (!ok) {
                return ::absl::AlreadyExistsError("Producer already exists");
            }

            key_views_.keys.push_back(it->first);
            return ::absl::OkStatus();
        }

        [[nodiscard]]
        std::optional<expr::runtime::CelValue>
        Get(google::protobuf::Arena* arena, expr::runtime::CelValue key) const override {
            if (key.type() != expr::runtime::CelValue::Type::kString) {
                return expr::runtime::CelValue::CreateError(&NotImplementedError);
            }
            const auto name = key.StringOrDie().value();

            // Cache hit
            if (const auto cached = cache_.find(name); cached != cache_.end()) {
                return cached->second;
            }

            // Produce
            const auto it = producers_.find(name);
            if (it == producers_.end()) {
                return ::absl::nullopt;
            }

            // NOTE: CelValue is non-owning and can be copied
            // NOTE: producer ensures that the value is not dangling
            expr::runtime::CelValue value = it->second->Produce(arena);
            cache_.emplace(name, value);
            return value;
        }

        [[nodiscard]]
        ::absl::StatusOr<bool> Has(const expr::runtime::CelValue& key) const override {
            if (key.type() != expr::runtime::CelValue::Type::kString) {
                return false;
            }
            return producers_.contains(key.StringOrDie().value());
        }

        [[nodiscard]]
        int size() const override {
            return static_cast<int>(producers_.size());
        }

        [[nodiscard]]
        ::absl::StatusOr<const expr::runtime::CelList*> ListKeys() const override {
            return &key_views_;
        }

        [[nodiscard]]
        std::optional<expr::runtime::CelValue> operator[](expr::runtime::CelValue key) const override {
            return expr::runtime::CelValue::CreateError(&NotImplementedError);
        }

    private:
        struct KeyHash final : std::hash<std::string_view> {
            using is_transparent = void;
        };
        struct KeyEqual final : std::equal_to<std::string_view> {
            using is_transparent = void;
        };

        template <typename Value>
        using ScaleDict = std::unordered_map<std::string, Value, KeyHash, KeyEqual>;

    private:
        /// @note The collection can only grow.
        /// @note The collection must reject duplicates.
        ScaleDict<std::unique_ptr<expr::runtime::CelValueProducer>> producers_;

        /// @note std::unordered_map has pointer stability guarantees, so we can use pointers to the keys.
        LazyCollectionKeyViews key_views_;

        /// @note producers return **non-owning** @c CelValue which can be cached by producer definition.
        mutable ScaleDict<expr::runtime::CelValue> cache_;
    };
} // namespace

TEST_F(CelExpressionsTest, ValueProducerLazyCollection) {
    // Stage: declarations
    const auto add_declarations = [&](::cel::TypeCheckerBuilder& builder) -> ::absl::Status {
        const cel::StringType key_type;
        const cel::IntType value_type;
        const cel::MapType map_type{builder.arena(), key_type, value_type};
        CEL_RETURN_IF_ERROR( //
            builder.AddVariable(::cel::MakeVariableDecl("lazy", map_type))
        );
        return ::absl::OkStatus();
    };

    // Stage: definitions
    const auto add_definitions = [&](::expr::runtime::CelExpressionBuilder& builder) -> ::absl::Status {
        return ::absl::OkStatus();
    };

    // Stage: activation preparation
    const auto activation_or = [&]() -> ::absl::StatusOr<expr::runtime::Activation> {
        expr::runtime::Activation activation;

        auto* lazy = google::protobuf::Arena::Create<LazyCollection>(&arena_);

        CEL_RETURN_IF_ERROR( //
            lazy->Set("a", std::make_unique<SimpleIntValueProducer>(7))
        );
        CEL_RETURN_IF_ERROR( //
            lazy->Set("b", std::make_unique<CallbackValueProducer>([callback_calls = 0]() mutable -> int64_t {
                return 10 * ++callback_calls; // 10, 20, ...
            }))
        );

        activation.InsertValue("lazy", expr::runtime::CelValue::CreateMap(lazy));
        return activation;
    }();
    ASSERT_TRUE(activation_or.ok()) << activation_or.status().message();

    // Stage: execution
    struct TestCase final {
        std::string_view expression;
        expr::runtime::CelValue::Type expected_type;
        std::variant<int64_t, bool> expected_value;
    };
    const std::vector<TestCase> test_cases = {
        // membership and size
        TestCase{
            .expression = "'a' in lazy",
            .expected_type = expr::runtime::CelValue::Type::kBool,
            .expected_value = true,
        },
        TestCase{
            .expression = "'c' in lazy",
            .expected_type = expr::runtime::CelValue::Type::kBool,
            .expected_value = false,
        },
        TestCase{
            .expression = "size(lazy)",
            .expected_type = expr::runtime::CelValue::Type::kInt64,
            .expected_value = int64_t{2},
        },
        // cached value behavior: b + b -> 10 + 10 (not 10 + 20)
        TestCase{
            .expression = "lazy.b + lazy.b",
            .expected_type = expr::runtime::CelValue::Type::kInt64,
            .expected_value = int64_t{20},
        },
        TestCase{
            .expression = "lazy['b'] + lazy['b']",
            .expected_type = expr::runtime::CelValue::Type::kInt64,
            .expected_value = int64_t{20},
        },
        // missing key -> error
        TestCase{
            .expression = "lazy['missing']",
            .expected_type = expr::runtime::CelValue::Type::kError,
        },
        // normal retrieval
        TestCase{
            .expression = "lazy['a']",
            .expected_type = expr::runtime::CelValue::Type::kInt64,
            .expected_value = int64_t{7},
        },
    };
    for (const auto& tc : test_cases) {
        const auto result = EvaluateExpression(
            tc.expression, //
            activation_or.value(),
            add_declarations,
            add_definitions
        );

        ASSERT_TRUE(result.ok()) //
            << result.status().message() << " for expression: " << tc.expression;

        ASSERT_EQ(result->type(), tc.expected_type) //
            << " for expression: " << tc.expression << " got: " << result->DebugString();

        switch (tc.expected_type) {
            case expr::runtime::CelValue::Type::kError: {
                // Nothing else to assert for error values in these tests
                continue;
            }
            case expr::runtime::CelValue::Type::kInt64: {
                const auto expected = std::get<int64_t>(tc.expected_value);
                ASSERT_EQ(result->Int64OrDie(), expected) //
                    << " for expression: " << tc.expression;
                break;
            }
            case expr::runtime::CelValue::Type::kBool: {
                const auto expected_bool = std::get<bool>(tc.expected_value);
                ASSERT_EQ(result->BoolOrDie(), expected_bool) //
                    << " for expression: " << tc.expression;
                break;
            }
            default: {
                FAIL() << "Unexpected type: " << static_cast<int>(tc.expected_type);
                break;
            }
        }
    }
}
