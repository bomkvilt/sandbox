#include <coroutine>
#include <cstddef>
#include <exception>

#include "gtest/gtest.h"

/**
 * https://github.com/tilir/cpp-masters/blob/master/coroutines/natseq.cc
 *  Case study: simple c++ generator that
 *  - has a lazy initialization (initial_suspend() == std::suspend_always{})
 *  - caches a yielded value and allows to access the one multiple times
 */
namespace {
    template <typename T>
    class TGenerator final {
    public:
        struct promise_type final {
        public:
            using coro_handle = std::coroutine_handle<promise_type>;

        public:
            T current_value;

        public:
            auto get_return_object() {
                return coro_handle::from_promise(*this);
            }

            auto initial_suspend() {
                return std::suspend_always{};
            }

            auto final_suspend() noexcept {
                return std::suspend_always{};
            }

            void return_void() {
                // pass
            }

            void unhandled_exception() {
                std::terminate();
            }

            auto yield_value(T value) {
                current_value = std::move(value);
                return std::suspend_always{};
            }
        };

        using coro_handle = std::coroutine_handle<promise_type>;

    public:
        TGenerator(coro_handle h)
            : Handle_(h)
        {}

        TGenerator(TGenerator const &) = delete;

        TGenerator(TGenerator &&rhs)
            : Handle_(rhs.Handle_)
        {
            rhs.Handle_ = nullptr;
        }

        ~TGenerator() {
            if (Handle_) {
                Handle_.destroy();
            }
        }

        bool Next() {
            // NOTE: if the function is called when an underlined coro has already been finished
            // `Handle_.resume()` will try to acces already released memory, hence the `&& !Handle_.done()`
            return Handle_ && !Handle_.done() ? (Handle_.resume(), !Handle_.done()) : false;
        }

        T CurrentValue() const {
            return Handle_.promise().current_value;
        }

    private:
        coro_handle Handle_;
    };
}


namespace {
    TGenerator<int> NaturalNums(size_t end) {
        for (int num = 0; num < end; num += 1) {
            co_yield num;
        }
    }
}


TEST(cpp23_coro_generators_simple, simple_test) {
    static constexpr size_t NUMS_COUNT = 10;

    auto nums = ::NaturalNums(NUMS_COUNT);

    for (int i = 0, n = 0; i < NUMS_COUNT; ++i, ++n) {
        // generate and cache a next value
        GTEST_ASSERT_TRUE(nums.Next());

        // check the cached result
        const auto y = nums.CurrentValue();
        GTEST_ASSERT_EQ(y, n);
    }

    // inner cucle must be exited
    GTEST_ASSERT_FALSE(nums.Next());
    GTEST_ASSERT_FALSE(nums.Next());
}
