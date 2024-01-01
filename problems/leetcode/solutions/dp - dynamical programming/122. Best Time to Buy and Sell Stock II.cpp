#include <algorithm>
#include <array>
#include <iostream>
#include <vector>


class Solution {
public:
    int maxProfit(const std::vector<int>& prices) {
        using TDayState = std::array<int, 2>;

        enum EState {
            WITH_STOCK,
            WITHOUT_STOCK,
            __COUNT__,
        };

        if (prices.empty()) {
            return 0;
        }

        TDayState oldState;
        TDayState newState;

        std::fill(oldState.begin(), oldState.end(), 0);
        std::fill(newState.begin(), newState.end(), 0);

        auto* stateA = &oldState;
        auto* stateB = &newState;

        (*stateB)[WITH_STOCK] = (*stateA)[WITHOUT_STOCK] - prices[0];
        std::swap(stateA, stateB);

        for (size_t i = 1; i < prices.size(); ++i, std::swap(stateA, stateB)) {
            const auto& price = prices[i];
            (*stateB)[WITHOUT_STOCK] = std::max(
                (*stateA)[WITHOUT_STOCK],     // hold stock
                (*stateA)[WITH_STOCK] + price // by a stock
            );
            (*stateB)[WITH_STOCK] = std::max(
                (*stateA)[WITH_STOCK],           // hold stock
                (*stateA)[WITHOUT_STOCK] - price // by a stock
            );
        }

        return std::max((*stateA)[WITH_STOCK], (*stateA)[WITHOUT_STOCK]);
    }
};
