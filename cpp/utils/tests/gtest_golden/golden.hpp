#include <fstream>

#include <gmock/gmock.h>

namespace golden {
    class GoldenMatcher {
    public:
        explicit GoldenMatcher(std::string file)
            : file_{std::move(file)} {
            const bool update = [] -> bool {
                const char* env = std::getenv("UPDATE_GOLDEN");
                return env != nullptr && std::string_view{env} == "1";
            }();

            if (not update) {
                expected_ = [&, this] -> std::string {
                    std::ifstream in{file_};
                    if (not in.is_open()) {
                        return "";
                    }

                    std::stringstream buffer;
                    buffer << in.rdbuf();
                    return std::move(buffer).str();
                }();
            }
        }

        bool MatchAndExplain(const std::string& actual, ::testing::MatchResultListener* listener) const {
            if (not expected_) {
                std::ofstream out{file_, std::ios::out | std::ios::trunc};
                if (not out.is_open()) {
                    *listener << "\n\nupdate failed: cannot open file '" << file_ << "'";
                    return false;
                }

                out << actual;
                if (not out.good()) {
                    *listener << "\n\nupdate failed: cannot write to file '" << file_ << "'";
                    return false;
                }
                return true;
            } else { // NOLINT(readability-else-after-return)
                if (expected_.value().empty()) {
                    *listener << "\n\nfailed to read file '" << file_ << "'";
                    return false;
                }
                if (actual == expected_.value()) {
                    return true;
                }
                *listener << "\n\nbazel: run --test_env=UPDATE_GOLDEN=1 for golden set update";
                return false;
            }
        }

        void DescribeTo(::std::ostream* os) const {
            if (expected_) {
                *os << ::testing::PrintToString(*expected_);
            } else {
                *os << "<re-canonisation>";
            }
        }

        void DescribeNegationTo(::std::ostream* os) const {
            DescribeTo(os);
        }

    private:
        std::string file_;
        std::optional<std::string> expected_;
    };

    inline ::testing::PolymorphicMatcher<GoldenMatcher> Golden(std::string file) {
        return ::testing::PolymorphicMatcher{GoldenMatcher{std::move(file)}};
    }
} // namespace golden
