#include <coroutine>
#include <cstddef>
#include <exception>

#include "gtest/gtest.h"

/**
 * SA: https://github.com/tilir/cpp-masters/blob/master/coroutines/resumable.cc
 * Case study: coroutine suspends it's execution and get resumed by external code
 */
namespace {
    class TResumable final {
    public:
        struct promise_type final {
        public:
            using coro_handle = std::coroutine_handle<promise_type>;

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

            /**
             *  NOTE: can be not implemented since the coroutine functions should not use co_yield
             *  ```
             *  co_yield <value> ≅ co_await coroutine_promise.yield_value(<value>)
             *  ```
             *  NOTE: any attemt to yeild something will rise the compiler error
             *  ```
             *  No member named 'yield_value' in '(anonymous namespace)::TResumable::promise_type'clang(no_member)
             *  ```
             */
            // auto yield_value(T value) {
            //     return std::suspend_always{};
            // }
        };

        using coro_handle = std::coroutine_handle<promise_type>;

    public:
        TResumable(coro_handle h)
            : Handle_(h)
        {}

        TResumable(TResumable const &) = delete;

        TResumable(TResumable &&rhs)
            : Handle_(rhs.Handle_)
        {
            rhs.Handle_ = nullptr;
        }

        // TODO: rule of 5

        ~TResumable() {
            if (Handle_) {
                Handle_.destroy();
            }
        }

        bool Resume() {
            // NOTE: if the function is called when an underlined coro has already been finished
            // Handle_.resume() will try to acces already released memory, hence the `&& !Handle_.done()`
            return Handle_ && !Handle_.done() ? (Handle_.resume(), !Handle_.done()) : false;
        }

    private:
        coro_handle Handle_;
    };
}


namespace {
    class TUsefulUseless final {
    public:
        TResumable Action(size_t end) {
            for (int num = 0; num < end; ++num, ++Generations_) {
                co_await std::suspend_always{};
            }
        }

        int State() const noexcept {
            return Generations_;
        }

    private:
        int Generations_ = 0;
    };
}


GTEST_TEST(cpp23_coro_coroutines_await_resume, simple_test) {
    static constexpr size_t NUMS_COUNT = 10;

    auto object = ::TUsefulUseless{};
    auto nums = object.Action(NUMS_COUNT);

    for (int i = 0, n = 0; i < NUMS_COUNT; ++i, ++n) {
        // generate and cache a next value
        GTEST_ASSERT_TRUE(nums.Resume());

        // check the cached result
        GTEST_ASSERT_EQ(object.State(), n);
    }

    // inner cucle must be completed
    GTEST_ASSERT_FALSE(nums.Resume());

    // NOTE: co_await [NUMS_COUNT]; ++Generations_; State === Generations_
    GTEST_ASSERT_EQ(object.State(), NUMS_COUNT);
}
