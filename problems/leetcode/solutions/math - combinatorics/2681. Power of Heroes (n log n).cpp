#include <algorithm>
#include <cstdint>
#include <ranges>
#include <vector>


class Solution {
public:
    int sumOfPower(std::vector<int> nums) {
        static constexpr std::int64_t MOD = 1000'000'000 + 7;

        static constexpr auto mod_sum = [](const std::int64_t a, const std::int64_t b) -> std::int64_t {
            return (a + b) % MOD;
        };

        static constexpr auto mod_mul = [](const std::int64_t a, const std::int64_t b) -> std::int64_t {
            return (a * b) % MOD;
        };

        std::ranges::sort(nums);

        std::int64_t sum = 0;
        for (std::int64_t fi = 0; const auto xi : std::views::reverse(nums)) {
            const auto xi_2 = mod_mul(xi, xi);
            sum = mod_sum(sum, xi_2 * xi + xi * fi);
            fi = mod_sum(xi_2, fi + fi);
        }
        return sum;
    }
};
