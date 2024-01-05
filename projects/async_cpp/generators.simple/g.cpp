#include <coroutine>

#include "gtest/gtest.h"


namespace {
    template <typename T>
    struct generator {
        struct promise_type {
            using coro_handle = std::coroutine_handle<promise_type>;

            T current_value;

            auto get_return_object() {
                return coro_handle::from_promise(*this);
            }

            auto initial_suspend() {
                return std::suspend_always();
            }

            auto final_suspend() noexcept {
                return std::suspend_always();
            }

            void return_void() {
                // pass
            }

            void unhandled_exception() {
                std::terminate();
            }

            auto yield_value(T value) {
                current_value = value;
                return std::suspend_always{};
            }
        };

        using coro_handle = std::coroutine_handle<promise_type>;

        bool move_next() {
            return handle_ ? (handle_.resume(), !handle_.done()) : false;
        }

        T current_value() const {
            return handle_.promise().current_value;
        }

        generator(coro_handle h)
            : handle_(h)
        {
        }

        generator(generator const &) = delete;

        generator(generator &&rhs)
            : handle_(rhs.handle_)
        {
            rhs.handle_ = nullptr;
        }

        ~generator() {
            if (handle_) {
                handle_.destroy();
            }
        }

    private:
        coro_handle handle_;
    };
}


namespace {
    generator<int> natural_nums() {
        int num = 0;
        for (;;) {
            co_yield num;
            num += 1;
        }
    }
}


TEST(coroutines, natseq) {
    int n = 0;
    auto nums = natural_nums();

    for (int i = 0; i < 10; ++i) {
        nums.move_next();
        auto y = nums.current_value();
        EXPECT_EQ(y, n);
        n = n + 1;
    }
}
