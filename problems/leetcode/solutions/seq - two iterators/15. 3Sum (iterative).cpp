#include <vector>
#include <algorithm>


class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums, const int target = 0) {
        if (nums.size() < 3) {
            return {};
        }

        std::ranges::sort(nums);

        if (nums.front() > 0 || nums.back() < 0) {
            return {};
        }

        auto forward = [&nums](auto& it, auto end) -> void {
            for (const auto& base = *it; ++it != end && *it == base; ) {}
        };

        auto result = std::vector<std::vector<int>>{};
        for (auto it1 = nums.begin(); it1 < nums.end() && *it1 <= 0; forward(it1, nums.end())) {
            for (auto [it2, it3] = std::tuple(it1 + 1, nums.rbegin()); it2 < it3.base() - 1; ) { // NOTE: revence iterator
                const auto sum = *it1 + *it2 + *it3;
                if (sum > target) {
                    forward(it3, nums.rend());
                    continue;
                }
                if (sum < target) {
                    forward(it2, nums.end());
                    continue;
                }
                result.push_back({*it1, *it2, *it3});
                forward(it2, nums.end());
            }
        }

        return result;
    }
};
