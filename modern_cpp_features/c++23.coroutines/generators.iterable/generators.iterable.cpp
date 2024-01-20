
#include "gtest/gtest.h"

#include <coroutine>
#include <cstddef>
#include <exception>
#include <iterator>
#include <memory>
#include <ranges>


/**
 * SA: https://github.com/tilir/cpp-masters/blob/master/coroutines/natseq_range.cc
 * Case study: simple c++ generator that can be iterated
 */
namespace {
    template <typename T>
    class TGeneratorV2 final: public std::ranges::view_interface<TGeneratorV2<T>> {
    public:
        struct promise_type final {
        public:
            using coro_handle = std::coroutine_handle<promise_type>;

        public:
            const T* current_value;

        public:
            auto get_return_object() noexcept {
                return coro_handle::from_promise(*this);
            }

            // NOTE: lazy initialisation
            auto initial_suspend() noexcept {
                return std::suspend_always{};
            }

            // NOTE: co_yield x; x must be accessable
            auto final_suspend() noexcept {
                return std::suspend_always{};
            }

            void return_void() noexcept {
                // pass
            }

            void unhandled_exception() {
                std::terminate();
            }

            auto yield_value(const T& value) {
                // NOTE: operator& can be overridden
                current_value = std::addressof(value);
                return std::suspend_always{};
            }
        };

        using coro_handle = std::coroutine_handle<promise_type>;

    public:
        // NOTE: c++20 ranges style iterator
        // NOTE: set Handle_=nullptr to implement a default iterator concept
        // SA: https://en.cppreference.com/w/cpp/coroutine/coroutine_handle#Example
        struct TIterator final {
        public:
            using iterator_category = std::input_iterator_tag;

            // NOTE: it's assumed that the iterator is a const iterator
            using value_type = T;
            using reference = const T&;
            using pointer = const T*;

            using difference_type = std::ptrdiff_t;

        public:
            TIterator(coro_handle h) noexcept
                : Handle_(h)
            {}

            // NOTE: operator*()'s result will be invalid after operator++ invocation
            TIterator& operator++() {
                Handle_.resume();
                return *this;
            }

            // NOTE: the method must be implemented to satisfy an std::input_iterator concept
            // NOTE: postfix iterator makes no sense
            void operator++(int) {
                ++(*this);
            }

            reference operator*() const {
                return *Handle_.promise().current_value;
            }

            pointer operator->() const {
                return Handle_.promise().current_value;
            }

            bool operator==(std::default_sentinel_t) const {
                return !Handle_ || Handle_.done();
            }

        private:
            coro_handle Handle_;
        };

        static_assert(std::input_iterator<TIterator>);

    public:
        TGeneratorV2(coro_handle h)
            : Handle_(h) // NOTE: struct with a not owning ptr
        {}

        TGeneratorV2(const TGeneratorV2&) = delete;

        TGeneratorV2(TGeneratorV2&& rhs)
            : Handle_(rhs.Handle_) // NOTE: ~ POD
        {
            rhs.Handle_ = nullptr;
        }

        TGeneratorV2& operator=(const TGeneratorV2&) = delete;

        TGeneratorV2& operator=(TGeneratorV2&& r) noexcept {
            if (this != std::addressof(r)) {
                Handle_ = r.Handle_; // NOTE: ~ POD
                r.Handle_ = nullptr;
            }
            return *this;
        }

        ~TGeneratorV2() {
            if (Handle_) {
                Handle_.destroy();
            }
        }

    public:
        TIterator begin() noexcept {
            // NOTE: the behaviour allows to
            // - keep initialisation lazy
            // - generate identical iterator from the same inner coro state
            if (Handle_ && !Initialized) {
                Initialized = true;
                Handle_.resume();
            }
            return TIterator{Handle_};
        }

        std::default_sentinel_t end() const noexcept {
            return {};
        }

    private:
        bool Initialized = false;
        coro_handle Handle_;
    };

    static_assert(std::ranges::range<TGeneratorV2<int>>);
    static_assert(std::ranges::view<TGeneratorV2<int>>);
    static_assert(std::ranges::viewable_range<TGeneratorV2<int>>);
}


#ifndef ERROR
    #define ERROR 0
#endif

namespace {
    TGeneratorV2<int> NaturalNums(size_t end) {
        for (int num = 0; num < end; num += 1) {
            co_yield num;
        }
    }
}


GTEST_TEST(cpp23_coro_generators_iterable, simple_test) {
    static constexpr size_t NUMS_COUNT = 10;
    static constexpr size_t FIRST_ROUND = 5;

#if ERROR
    {
        // NOTE: view composition requires rvalues ref (&&) or copiable objects
        auto nums = ::NaturalNums(NUMS_COUNT);
        for (const auto& value : std::views::take(nums, FIRST_ROUND)) {
            // pass
        }
    }
#endif
    {
        // the safest generator usage
        auto nums = ::NaturalNums(NUMS_COUNT) | std::views::take(FIRST_ROUND);
        for (size_t n = 0; const auto& value : nums) {
            GTEST_ASSERT_EQ(value, n++);
        }
        GTEST_ASSERT_NE(nums.begin(), nums.end());
        GTEST_ASSERT_EQ(*nums.begin(), FIRST_ROUND);
    }
    {
        // hack to a use move-only range in multiple chains
        auto nums = ::NaturalNums(NUMS_COUNT);

        for (size_t n = 0; const auto& value : std::ranges::subrange(nums) | std::views::take(FIRST_ROUND)) {
            GTEST_ASSERT_EQ(value, n++);
        }
        GTEST_ASSERT_NE(nums.begin(), nums.end());
        GTEST_ASSERT_EQ(*nums.begin(), FIRST_ROUND);

        for (size_t n = FIRST_ROUND; const auto& value : std::ranges::subrange(nums)) {
            GTEST_ASSERT_EQ(value, n++);
        }
        GTEST_ASSERT_EQ(nums.begin(), nums.end());
        GTEST_ASSERT_EQ(*nums.begin(), NUMS_COUNT);
    }
}

#undef ERROR
