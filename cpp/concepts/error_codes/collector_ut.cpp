#include <system_error>

#include <gtest/gtest.h>

#include "./collector.hpp"
#include "cpp/utils/tests/gtest_golden/golden.hpp"

// =================================================================================================

namespace domain_a {
    enum class ECode : std::uint8_t {
        kInvalid,
        kMissing,
    };

    struct ErrorCode : std::error_category {
        [[nodiscard]]
        const char* name() const noexcept override {
            return "domain_a";
        }

        [[nodiscard]]
        std::string message(int ev) const override {
            switch (static_cast<ECode>(ev)) {
                case ECode::kInvalid: {
                    return "invalid";
                }
                case ECode::kMissing: {
                    return "missing";
                }
                default: {
                    return "unknown";
                }
            }
        }
    };

    [[nodiscard]]
    std::error_code make_error_code(ECode code) {
        static const ErrorCode instance;
        return std::error_code{static_cast<int>(code), instance};
    }
} // namespace domain_a

// =================================================================================================

namespace domain_b {
    enum class ECode : std::uint8_t {
        kRed,
        kBlack,
    };

    struct ErrorCode : std::error_category {
        [[nodiscard]]
        const char* name() const noexcept override {
            return "domain_b";
        }

        [[nodiscard]]
        std::string message(int ev) const override {
            switch (static_cast<ECode>(ev)) {
                case ECode::kRed: {
                    return "red";
                }
                case ECode::kBlack: {
                    return "black";
                }
                default: {
                    return "unknown";
                }
            }
        }
    };

    [[nodiscard]]
    std::error_code make_error_code(ECode code) {
        static const ErrorCode instance;
        return std::error_code{static_cast<int>(code), instance};
    }
} // namespace domain_b

// =================================================================================================

namespace tags {
    struct ErrorTagA final {
        std::error_code code;
        error_codes::TypeIdA id;

        ErrorTagA(std::error_code code, error_codes::TypeIdA id)
            : code{code}
            , id{id} {
        }

        void operator()(error_codes::ErrorCollector& collector) const {
            collector.errors_a[code].insert(id);
        }

        void operator()(std::ostream& os) const {
            std::format_to(std::ostream_iterator<char>{os}, "{} a <{}>: ", code.message(), id);
        }
    };
    static_assert(error_codes::error_tag<ErrorTagA>);

    struct ErrorTagB final {
        std::error_code code;
        error_codes::TypeIdB id;

        ErrorTagB(std::error_code code, error_codes::TypeIdB id)
            : code{code}
            , id{std::move(id)} {
        }

        void operator()(error_codes::ErrorCollector& collector) const {
            collector.errors_b[code].insert(id);
        }

        void operator()(std::ostream& os) const {
            std::format_to(std::ostream_iterator<char>{os}, "{} b <{}>: ", code.message(), id);
        }
    };
    static_assert(error_codes::error_tag<ErrorTagB>);
} // namespace tags

// =================================================================================================

class ErrorCollectorTest : public ::testing::Test {
protected:
    error_codes::ErrorCollector collector_;

protected:
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

// =================================================================================================

// NOLINTBEGIN(readability-magic-numbers)
TEST_F(ErrorCollectorTest, SimpleTest) {
    collector_.add_error()
        .with_tag(tags::ErrorTagA{domain_a::make_error_code(domain_a::ECode::kInvalid), 123})
        .text("test message: {}", 123)
        .note("just give up: {}", 456)
        .note("just give up: {}", 789);

    collector_.add_error()
        .with_tag(tags::ErrorTagA{domain_a::make_error_code(domain_a::ECode::kMissing), 456})
        .text("test message: {}", 985);

    collector_.add_error()
        .with_tag(tags::ErrorTagB{domain_b::make_error_code(domain_b::ECode::kRed), "789"})
        .text("test message: {}", 123)
        .note("just give up: {}", "789");

    // Test report generation
    EXPECT_THAT(collector_.format_report(), golden::Golden(test_file()));

    // Test error classification
    {
        const auto error = domain_a::make_error_code(domain_a::ECode::kInvalid);
        const auto errors = collector_.errors_a[error];
        EXPECT_THAT(errors, testing::UnorderedElementsAre(123));
    }
    {
        const auto error = domain_a::make_error_code(domain_a::ECode::kMissing);
        const auto errors = collector_.errors_a[error];
        EXPECT_THAT(errors, testing::UnorderedElementsAre(456));
    }
    {
        const auto error = domain_b::make_error_code(domain_b::ECode::kRed);
        const auto errors = collector_.errors_b[error];
        EXPECT_THAT(errors, testing::UnorderedElementsAre("789"));
    }
}
// NOLINTEND(readability-magic-numbers)
