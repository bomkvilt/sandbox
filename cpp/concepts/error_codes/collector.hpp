#pragma once

#include <algorithm>
#include <cstdint>
#include <format>
#include <ostream>
#include <sstream>
#include <system_error>
#include <unordered_map>
#include <unordered_set>

#define LIFETIME_BOUND [[clang::lifetimebound]]

namespace error_codes {
    struct ErrorCollector;
}

namespace error_codes::internal {
    enum class EBuilderStage : std::uint8_t {
        kTags,
        kText,
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
        using Self = ErrorBuilder<kStage>;

        ErrorBuilder()
        requires(kStage == EBuilderStage::kTags)
        = default;

        explicit ErrorBuilder(ErrorCollector* collector LIFETIME_BOUND, const bool need_logging)
        requires(kStage == EBuilderStage::kTags)
            : need_logging_{need_logging}
            , collector_{collector} {
        }

        ErrorBuilder(const ErrorBuilder&) = delete;
        ErrorBuilder& operator=(ErrorBuilder&&) = delete;
        ErrorBuilder& operator=(const ErrorBuilder&) = delete;

        ~ErrorBuilder() noexcept {
            try {
                flush();
            } catch (...) {
                // ignore
            }
        }

        template <error_tag Tag>
        requires(kStage == EBuilderStage::kTags)
        [[nodiscard]]
        ErrorBuilder<EBuilderStage::kTags> //
        with_tag(this Self&& self LIFETIME_BOUND, const Tag& tag) {
            if (self.collector_) {
                tag(*self.collector_);
            }
            if (self.need_logging_) {
                tag(self.ss_);
            }
            return std::move(self);
        }

        template <typename... Args>
        requires(kStage == EBuilderStage::kText or kStage == EBuilderStage::kTags)
        ErrorBuilder<EBuilderStage::kText>
        text(this Self&& self LIFETIME_BOUND, const std::format_string<Args...> fmt, Args&&... args) {
            if (self.need_logging_) {
                std::format_to(std::ostreambuf_iterator{self.ss_}, fmt, std::forward<Args>(args)...);
            }
            return std::move(self);
        }

        template <typename... Args>
        requires(kStage == EBuilderStage::kNotes or kStage == EBuilderStage::kText)
        ErrorBuilder<EBuilderStage::kNotes>
        note(this Self&& self LIFETIME_BOUND, const std::format_string<Args...> fmt, Args&&... args) {
            if (self.need_logging_) {
                self.ss_ << "\nnote: ";
                std::format_to(std::ostreambuf_iterator{self.ss_}, fmt, std::forward<Args>(args)...);
            }
            return std::move(self);
        }

        void flush(this Self&& self) {
            self.flush();
        }

    private:
        template <EBuilderStage kS>
        friend class ErrorBuilder;

        template <EBuilderStage kS>
        ErrorBuilder(ErrorBuilder<kS>&& other) {
            swap(other);
        }

        template <EBuilderStage kS>
        void swap(ErrorBuilder<kS>& other) {
            std::swap(need_logging_, other.need_logging_); // noexcept
            std::swap(collector_, other.collector_);       // noexcept
            std::swap(ss_, other.ss_);                     // noexcept(false)
        }

        void flush(this Self& self);

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
        using Self = ErrorCollector;

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

        [[nodiscard]]
        ErrorBuilder add_error(this Self& self LIFETIME_BOUND) {
            return ErrorBuilder{&self, self.need_logging};
        }

        [[nodiscard]]
        std::string format_report(this const Self& self);
    };
} // namespace error_codes

// =================================================================================================

namespace error_codes::internal {
    template <EBuilderStage stage>
    void ErrorBuilder<stage>::flush(this Self& self) {
        if (self.collector_ != nullptr and self.need_logging_) {
            self.collector_->errors.insert(std::move(self.ss_).str());
        }
        self.collector_ = nullptr;
        self.ss_.clear();
    }
} // namespace error_codes::internal
