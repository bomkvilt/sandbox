#include <algorithm>
#include <cstddef>
#include <ranges>
#include <vector>


class Solution {
public:
    int maxCoins(const std::vector<int>& nums) {
        const auto st = CopyWithPadding(nums, 1);

        const auto n = nums.size();
        const auto N = nums.size() + 2;
        std::vector<int> dp (N * N, int{});

        for (size_t i = n; i >= 1; --i) {
            for (const auto j : std::views::iota(i, n + 1)) {
                int maxReward = 0;

                for (const auto k : std::views::iota(i, j + 1)) {
                    const auto lastBurst = st[i - 1] * st[k] * st[j + 1];
                    const auto reward = lastBurst + dp[i * N + k - 1] + dp[(k + 1) * N + j];
                    maxReward = std::max(maxReward, reward);
                }

                dp[i * N + j] = maxReward;
            }
        }

        return dp[1 * N + n];
    }

private:
    std::vector<int> CopyWithPadding(const std::vector<int>& nums, const int value) {
        std::vector<int> st;
        st.reserve(nums.size() + 2);

        // NOTE: since c++26
        // std::ranges::copy(std::views::concat({value}, nums, {value}), std::back_inserter(st));

        st.push_back(value);
        std::ranges::copy(nums, std::back_inserter(st));
        st.push_back(value);

        return st;
    }
};
