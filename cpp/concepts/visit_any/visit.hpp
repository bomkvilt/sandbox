#pragma once

#include <algorithm>
#include <any>
#include <array>
#include <format>
#include <optional>
#include <string>

namespace visit_any::internal {
    struct NoArgs final {};

    template <typename... Args>
    struct FirstArg final {
        using type = typename std::tuple_element<0, std::tuple<Args...>>::type;
    };

    template <>
    struct FirstArg<> final {
        using type = NoArgs;
    };

    // =====================================================

    template <typename T>
    struct FunctionTraits;

    // Specialization for lambda/function objects
    template <typename T>
    struct FunctionTraits {
    private:
        using Impl = FunctionTraits<decltype(&T::operator())>;

    public:
        using FirstArg = typename Impl::FirstArg;
        using Return = typename Impl::Return;
    };

    // Specialization for member function pointers (const)
    template <typename T, typename R, typename... Args>
    struct FunctionTraits<R (T::*)(Args...) const> {
        using FirstArg = typename FirstArg<Args...>::type;
        using Return = R;
    };

    // Specialization for member function pointers (mut)
    template <typename T, typename R, typename... Args>
    struct FunctionTraits<R (T::*)(Args...)> {
        using FirstArg = typename FirstArg<Args...>::type;
        using Return = R;
    };

    // Specialization for function pointers
    template <typename R, typename... Args>
    struct FunctionTraits<R (*)(Args...)> {
        using FirstArg = typename FirstArg<Args...>::type;
        using Return = R;
    };

    // =====================================================

    template <typename F>
    consteval bool is_default() {
        return std::is_same_v<typename FunctionTraits<F>::FirstArg, NoArgs>;
    }

    template <typename... Fs>
    consteval bool at_most_one_default() {
        const std::array<bool, sizeof...(Fs)> defaults{is_default<Fs>()...};
        return std::ranges::count(defaults, true) <= 1;
    }

    template <typename... Fs>
    consteval bool default_last() {
        const std::array<bool, sizeof...(Fs)> defaults{is_default<Fs>()...};
        return std::ranges::count(defaults, true) == 0UL or defaults.back();
    }

    template <typename... Fs>
    struct Overloads : Fs... {
        using Fs::operator()...;
    };

    // =====================================================

    class BadAnyCast final : public std::bad_any_cast {
    public:
        explicit BadAnyCast(std::string msg)
            : msg_{std::move(msg)} {
        }

        [[nodiscard]]
        const char* what() const noexcept override {
            return msg_.c_str();
        }

    private:
        std::string msg_;
    };

    // =====================================================

    // Generic implementation
    template <typename R>
    struct VisitAnyImpl final {
        template <typename A, typename T, typename Callable>
        static std::optional<R> try_type(A& any, const Callable& overloads) {
            if (any.type() == typeid(T)) {
                return overloads(std::any_cast<T>(any));
            }
            return std::nullopt;
        }

        template <typename A, typename T, typename Callable>
        requires std::same_as<T, NoArgs>
        static std::optional<R> try_type(A& any, const Callable& overloads) {
            return overloads();
        }

        template <typename F, typename A, typename Callable>
        static std::optional<R> try_dispatch(A& any, const Callable& overloads) {
            using T = typename FunctionTraits<F>::FirstArg;
            return try_type<A, T>(any, overloads);
        }

        template <typename A, typename... Fs>
        requires std::same_as<std::remove_cvref_t<A>, std::any>
        R operator()(A&& any, const Overloads<Fs...>& overloads) const {
            std::optional<R> result;
            ((result = try_dispatch<Fs>(any, overloads)) || ...);

            if (result) {
                return *result;
            }
            throw BadAnyCast{std::format("Bad any cast: {}", any.type().name())};
        }
    };

    // Specialization for void-returning functions
    template <>
    struct VisitAnyImpl<void> final {
        template <typename A, typename T, typename Callable>
        static bool try_type(A& any, const Callable& overloads) {
            if (any.type() == typeid(T)) {
                overloads(std::any_cast<T>(any));
                return true;
            }
            return false;
        }

        template <typename A, typename T, typename Callable>
        requires std::same_as<T, NoArgs>
        static bool try_type(A& any, const Callable& overloads) {
            overloads();
            return true;
        }

        template <typename F, typename A, typename Callable>
        static bool try_dispatch(A& any, const Callable& overloads) {
            using T = typename FunctionTraits<F>::FirstArg;
            return try_type<A, T>(any, overloads);
        }

        template <typename A, typename... Fs>
        requires std::same_as<std::remove_cvref_t<A>, std::any>
        void operator()(A&& any, const Overloads<Fs...>& overloads) const {
            if ((try_dispatch<Fs>(any, overloads) || ...)) {
                return;
            }
            throw BadAnyCast{std::format("Bad any cast: {}", any.type().name())};
        }
    };

    // =====================================================

    /// ```cpp
    /// return visit_any(any, Overloads{
    ///   [&](const TA& spec) -> bool {
    ///     return f(spec);
    ///   },
    ///   [&](const TB& spec) -> bool {
    ///     return f(spec);
    ///   },
    ///   [&]() -> bool { // default, optional clause
    ///     return true;
    ///   },
    /// });
    /// ```
    template <typename A, typename... Fs>
    requires requires {
        requires std::same_as<std::remove_cvref_t<A>, std::any>;
        requires at_most_one_default<Fs...>();
        requires default_last<Fs...>();
    }
    [[maybe_unused]]
    auto visit_any(A&& any, const Overloads<Fs...>& overloads) {
        using TF = typename std::tuple_element_t<0, std::tuple<Fs...>>;
        using TR = typename FunctionTraits<TF>::Return;
        return VisitAnyImpl<TR>{}(std::forward<A>(any), overloads);
    }
} // namespace visit_any::internal

namespace visit_any {
    using internal::BadAnyCast;
    using internal::Overloads;
    using internal::visit_any;
} // namespace visit_any
