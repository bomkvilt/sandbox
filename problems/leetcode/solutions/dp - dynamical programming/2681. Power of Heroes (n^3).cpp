#include <algorithm>
#include <cstdint>
#include <numeric>
#include <ranges>
#include <vector>


class Solution {
public:
    /**
     * NOTE: The algo's time complexity is O(n^3), wich is more that required O(n log n)
     *
     * Here we try to fold the problem to [{num_heroes, min, max} -> occurrences] matrix
     * - Once we sort the array, the problems maps to {num_heroes, i, j} -> occurrences
     * - We add a new iterator k to allready existing group;
     *      - thus {num_heroes + 1, i, k} += {num_heroes, i, j}
     *      - thus {num_heroes + 1, i, k} = sum {j \in 0..n} {num_heroes, i, j}
     */
    int sumOfPower(std::vector<int> nums) {
        static constexpr int MOD = 1000'000'000 + 7;

        static constexpr auto mod_sum = [](const std::int64_t a, const std::int64_t b) -> std::int64_t {
            return (a + b) % MOD;
        };

        static constexpr auto mod_sub = [](const std::int64_t a, const std::int64_t b) -> std::int64_t {
            return (a + MOD - b) % MOD;
        };

        static constexpr auto calc_power = [](const std::int64_t min, const std::int64_t max) -> std::int64_t {
            return (((max * max) % MOD) * min) % MOD;
        };

        std::ranges::sort(nums);

        auto total_power = std::ranges::fold_left(
            nums | std::views::transform([](const auto x) { return calc_power(x, x); } ),
            std::int64_t{},
            std::plus{}
        );

        std::vector<std::int64_t> row_state;
        for (size_t i = 0; i < nums.size(); ++i) {
            row_state.clear();
            row_state.resize(nums.size());
            row_state[i] = 1;

            // round = num_heroes - 2
            for (const auto round : std::views::iota(0ul, nums.size() - i)) {
                auto total_sum = std::accumulate(
                    row_state.begin() + i + round,
                    row_state.end(),
                    std::int64_t{},
                    mod_sum
                );

                for (size_t k = nums.size() - 1; k > i + round; --k) {
                    total_sum = mod_sub(total_sum, row_state[k]);
                    total_power = mod_sum(total_power, (calc_power(nums[i], nums[k]) * total_sum) % MOD);
                    row_state[k] = total_sum;
                } 
            }
        }

        return total_power;
    }
};
