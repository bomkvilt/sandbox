#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <assert.h>


namespace {
    enum class EMode : int {
        HOLD_1, HOLD_0, SELL, BUY, __COUNT__,
    };

    struct TIdentityTransaction : public std::true_type {
        int operator()(int currentPrice) const {
            return 0;
        }
    };

    template<EMode from, EMode to>
    struct TTransaction : public std::false_type {
        static constexpr bool IsActive = false;

        int operator()(int currentPrice) const {
            throw std::logic_error("non implemented transaction");
        }
    };

    template<>
    struct TTransaction<EMode::HOLD_0, EMode::HOLD_0> : public TIdentityTransaction {};

    template<>
    struct TTransaction<EMode::HOLD_0, EMode::BUY> : public std::true_type {
        int operator()(int currentPrice) const {
            return -currentPrice;
        }
    };

    template<>
    struct TTransaction<EMode::BUY, EMode::HOLD_1> : public TIdentityTransaction {};

    template<>
    struct TTransaction<EMode::BUY, EMode::SELL> : public std::true_type {
        int operator()(int currentPrice) const {
            return currentPrice;
        }
    };

    template<>
    struct TTransaction<EMode::HOLD_1, EMode::SELL> : public std::true_type {
        int operator()(int currentPrice) const {
            return currentPrice;
        }
    };

    template<>
    struct TTransaction<EMode::HOLD_1, EMode::HOLD_1> : public TIdentityTransaction {};

    template<>
    struct TTransaction<EMode::SELL, EMode::HOLD_0> : public TIdentityTransaction {};
}


class Solution {
private:
    struct TState {
        int Deposit = 0;
        bool Active = false;
    };

    static constexpr size_t ModesCount = (size_t)EMode::__COUNT__;
    using TStateMap = std::array<TState, ModesCount>;

public:
    int maxProfit(const std::vector<int>& prices) {
        auto newStateMap = TStateMap{};
        auto oldStateMap = newStateMap;

        oldStateMap[(size_t)EMode::HOLD_0].Active = true;

        auto* stateA = &oldStateMap;
        auto* stateB = &newStateMap;

        constexpr static auto modes = std::make_index_sequence<ModesCount>{};
        for (int i = 0; i < prices.size(); ++i) {
            if (i > 0) {
                std::swap(stateA, stateB);
            }

            const auto& price = prices[i];
            MakeStateMapIteration(modes, *stateA, *stateB, price);
        }

        const auto max = std::max_element(
            stateB->begin(),
            stateB->end(),
            [](const TState& a, const TState& b) {
                return a.Deposit < b.Deposit;
            }
        );
        return max->Deposit;
    }

private:
    template<typename T, T... modeIds>
    void MakeStateMapIteration(
        std::integer_sequence<T, modeIds...> modeIdsSeq,
        const TStateMap& oldStateMap,
        TStateMap& newStateMap,
        const int currentPrice
    ) {
        ((
            MakeStateMapIteration<T, modeIds, modeIds...>(
                modeIdsSeq, oldStateMap, newStateMap, currentPrice
            )
        ), ...);
    }

    template<typename T, T modeIdTo, T... modeIds>
    void MakeStateMapIteration(
        std::integer_sequence<T, modeIds...> modeIdsSeq,
        const TStateMap& oldStateMap,
        TStateMap& newStateMap,
        const int currentPrice
    ) {
        constexpr auto DEFAULT_VALUE = INT_MIN;
        int candidate = DEFAULT_VALUE;

        const auto calcSource = [&]<T modeIdFrom>() -> void {
            constexpr auto modeFrom = EMode(modeIdFrom);
            constexpr auto modeTo = EMode(modeIdTo);

            using TCurrentTransation = TTransaction<modeFrom, modeTo>;
            if constexpr (TCurrentTransation::value) {
                if (const auto& oldState = oldStateMap[modeIdFrom]; oldState.Active) {
                    const auto delta = TCurrentTransation()(currentPrice);
                    candidate = std::max(candidate, oldState.Deposit + delta);
                }
            }
        };

        ((  // calc all inputs for a target stage
            calcSource.template operator()<modeIds>()
        ), ...);

        if (candidate != DEFAULT_VALUE) {
            newStateMap[modeIdTo].Deposit = candidate;
            newStateMap[modeIdTo].Active = true;
        }
        else {
            newStateMap[modeIdTo].Active = false;
        }
    }
};
