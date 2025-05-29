#pragma once

#include <algorithm>
#include <format>
#include <ostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <system_error>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define LIFETIME_BOUND [[clang::lifetimebound]]

namespace error_codes {
    struct ErrorCollector;
}

namespace error_codes::internal {
    enum class EBuilderStage {
        kTags,
        kFormat,
        kNotes,
    };

    // =====================================================

    template <typename T>
    concept error_tag = requires(ErrorCollector& collector, std::ostream& os) {
        { std::declval<const T>()(collector) } -> std::same_as<void>;
        { std::declval<const T>()(os) } -> std::same_as<void>;
    };

    // =====================================================

    template <EBuilderStage kStage>
    class ErrorBuilder final {
    public:
        ErrorBuilder()
        requires(kStage == EBuilderStage::kTags)
        = default;

        explicit ErrorBuilder(ErrorCollector* collector LIFETIME_BOUND, const bool need_logging)
        requires(kStage == EBuilderStage::kTags)
            : need_logging_{need_logging}
            , collector_{collector}
            , ss_{} {
        }

        ErrorBuilder(const ErrorBuilder&) = delete;
        ErrorBuilder& operator=(ErrorBuilder&&) = delete;
        ErrorBuilder& operator=(const ErrorBuilder&) = delete;

        ~ErrorBuilder() noexcept {
            flush();
        }

        template <error_tag Tag>
        requires(kStage == EBuilderStage::kTags)
        [[nodiscard]]
        ErrorBuilder<EBuilderStage::kTags> with_tag(const Tag& tag) &&
            LIFETIME_BOUND {
            if (collector_) {
                tag(*collector_);
            }
            if (need_logging_) {
                tag(ss_);
            }
            return ErrorBuilder<EBuilderStage::kTags>{std::move(*this)};
        }

        template <typename... Args>
        requires(kStage == EBuilderStage::kFormat or kStage == EBuilderStage::kTags)
        ErrorBuilder<EBuilderStage::kFormat> format(const std::format_string<Args...> fmt, Args&&... args) &&
            LIFETIME_BOUND {
            if (need_logging_) {
                std::format_to(std::ostreambuf_iterator{ss_}, fmt, std::forward<Args>(args)...);
            }
            return ErrorBuilder<EBuilderStage::kFormat>{std::move(*this)};
        }

        template <typename... Args>
        requires(kStage == EBuilderStage::kNotes or kStage == EBuilderStage::kFormat)
        ErrorBuilder<EBuilderStage::kNotes> note(const std::format_string<Args...> fmt, Args&&... args) &&
            LIFETIME_BOUND {
            if (need_logging_) {
                ss_ << "\n[note] ";
                std::format_to(std::ostreambuf_iterator{ss_}, fmt, std::forward<Args>(args)...);
            }
            return ErrorBuilder<EBuilderStage::kNotes>{std::move(*this)};
        }

    private:
        template <EBuilderStage kS>
        friend class ErrorBuilder;

        template <EBuilderStage kS>
        ErrorBuilder(ErrorBuilder<kS>&& other) noexcept {
            swap(other);
        }

        template <EBuilderStage kS>
        void swap(ErrorBuilder<kS>& other) noexcept {
            std::swap(need_logging_, other.need_logging_);
            std::swap(collector_, other.collector_);
            std::swap(ss_, other.ss_);
        }

        void flush() noexcept;

    private:
        bool need_logging_ = false;
        ErrorCollector* collector_ = nullptr;
        std::stringstream ss_;
    };
} // namespace error_codes::internal

namespace error_codes {
    using ErrorBuilder = internal::ErrorBuilder<internal::EBuilderStage::kTags>;
    using internal::error_tag;

    using TypeIdA = size_t;
    using TypeIdB = std::string;

    struct ErrorCollector final {
    public:
        bool need_logging = true;
        std::unordered_map<std::error_code, std::unordered_set<TypeIdA>> errors_a;
        std::unordered_map<std::error_code, std::unordered_set<TypeIdB>> errors_b;
        std::unordered_set<std::string> errors;

    public:
        ErrorCollector() = default;

        ErrorCollector(ErrorCollector&&) = default;
        ErrorCollector(const ErrorCollector&) = default;

        ErrorCollector& operator=(ErrorCollector&&) = default;
        ErrorCollector& operator=(const ErrorCollector&) = default;

        ErrorBuilder add_error() & LIFETIME_BOUND {
            return ErrorBuilder{this, need_logging};
        }

        std::string format_report() const {
            std::ostringstream ss;

            auto multiline = [re = std::regex{R"(\n)"}](std::string& record) -> void {
                record = std::regex_replace(record, re, "\n  ");
            };

            {
                auto sink = [&, called = false] mutable -> std::ostream& {
                    if (not called) {
                        called = true;
                        ss << "general errors:\n";
                    }
                    return ss;
                };
                for (const auto& [ec, ids] : errors_a) {
                    for (const auto& id : ids) {
                        sink() << "- " << ec.message() << " a <" << id << ">\n";
                    }
                }
                for (const auto& [ec, ids] : errors_b) {
                    for (const auto& id : ids) {
                        sink() << "- " << ec.message() << " b <" << id << ">\n";
                    }
                }
            }
            if (errors.size() > 0) {
                std::vector<std::string> records;
                records.reserve(errors.size());

                std::ranges::copy(errors, std::back_inserter(records));
                std::ranges::for_each(records, multiline);
                std::ranges::sort(records);

                ss << "detailed errors:\n";
                for (const auto& record : records) {
                    ss << "- " << record << "\n";
                }
            }
            return std::move(ss).str();
        }
    };
} // namespace error_codes

// =================================================================================================

namespace error_codes::internal {
    template <EBuilderStage stage>
    void ErrorBuilder<stage>::flush() noexcept {
        if (collector_ and need_logging_) {
            collector_->errors.insert(std::move(ss_).str());
            ss_.clear();
        }
    }
} // namespace error_codes::internal
