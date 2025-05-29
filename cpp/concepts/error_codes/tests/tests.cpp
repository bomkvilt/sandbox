#include <fstream>
#include <system_error>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../error_codes/collector.h"

namespace domain_a {
    enum class ECode {
        kInvalid,
        kMissing,
    };

    struct ErrorCode : std::error_category {
        const char* name() const noexcept override {
            return "domain_a";
        }

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

    std::error_code make_error_code(ECode code) {
        static const ErrorCode instance;
        return std::error_code{static_cast<int>(code), instance};
    }
} // namespace domain_a

namespace domain_b {
    enum class ECode {
        kRed,
        kBlack,
    };

    struct ErrorCode : std::error_category {
        const char* name() const noexcept override {
            return "domain_b";
        }

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

    std::error_code make_error_code(ECode code) {
        static const ErrorCode instance;
        return std::error_code{static_cast<int>(code), instance};
    }
} // namespace domain_b

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
            , id{id} {
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

MATCHER_P(Golden, file, "matches golden file content (--test_env=UPDATE_GOLDEN=1)") {
    static_assert(std::is_same_v<file_type, std::string> or std::is_same_v<file_type, std::string_view>);

    const bool update = [] -> bool {
        const char* env = std::getenv("UPDATE_GOLDEN");
        return env != nullptr && std::string_view{env} == "1";
    }();

    if (update) {
        std::ofstream out{file, std::ios::out | std::ios::trunc};
        if (not out.is_open()) {
            *result_listener << "update failed: cannot open file '" << file << "'";
            return false;
        }

        out << arg;
        if (not out.good()) {
            *result_listener << "update failed: cannot write to file '" << file << "'";
            return false;
        }
        return true;
    }

    const std::string expected = [&] -> std::string {
        std::ifstream in{file};
        if (not in.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << in.rdbuf();
        return std::move(buffer).str();
    }();

    if (expected.empty()) {
        *result_listener << "failed to read file '" << file << "'";
        return false;
    }

    return ::testing::ExplainMatchResult(::testing::Eq(expected), arg, result_listener);
}

class ErrorCollectorTest : public ::testing::Test {
protected:
    error_codes::ErrorCollector collector_;

protected:
    std::string test_name() const {
        const char* suite = ::testing::UnitTest::GetInstance()->current_test_suite()->name();
        const char* test = ::testing::UnitTest::GetInstance()->current_test_info()->name();
        return std::format("{}.{}", suite, test);
    }

    std::string test_file() const {
        constexpr std::string_view file = __FILE__;
        constexpr std::string_view base = file.substr(0u, file.find_last_of('/'));
        return std::format("{}/golden/{}", base, test_name());
    }
};

// =================================================================================================

TEST_F(ErrorCollectorTest, SimpleTest) {
    collector_.add_error()
        .with_tag(tags::ErrorTagA{domain_a::make_error_code(domain_a::ECode::kInvalid), 123})
        .format("test message: {}", 123)
        .note("help: just give up: {}", 456)
        .note("help: just give up: {}", 789);

    collector_.add_error()
        .with_tag(tags::ErrorTagA{domain_a::make_error_code(domain_a::ECode::kMissing), 456})
        .format("test message: {}", 985);

    collector_.add_error()
        .with_tag(tags::ErrorTagB{domain_b::make_error_code(domain_b::ECode::kRed), "789"})
        .format("test message: {}", 123)
        .note("help: just give up: {}", "789");

    // Test report generation
    EXPECT_THAT(collector_.format_report(), Golden(test_file()));

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
