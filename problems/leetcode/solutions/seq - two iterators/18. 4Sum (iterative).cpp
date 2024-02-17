// https://leetcode.com/problems/4sum/description/

#include <algorithm>
#include <vector>


class Solution {
public:
    std::vector<std::vector<int>> fourSum(std::vector<int>& nums, int target) {
        if (nums.size() < 4) {
            return {};
        }

        std::ranges::sort(nums);

        auto result = std::vector<std::vector<int>>();

        const auto I0 = 0;
        for (int i = I0; i < nums.size() - 3; ++i) {
            const auto a = nums[i];
            if (i > I0 && a == nums[i - 1]) {
                continue;
            }

            const auto J0 = i + 1;
            for (int j = J0; j < nums.size() - 2; ++j) {
                const auto b = nums[j];
                if (j > J0 && b == nums[j - 1]) {
                    continue;
                }

                const auto r = (int64_t) target - a - b;
                int p = j + 1;
                int q = nums.size() - 1;

                while (p < q) {
                    const auto c = nums[p];
                    const auto d = nums[q];
                    const auto s = c + d;

                    if (s < r) { ++p; continue; }
                    if (s > r) { --q; continue; }

                    result.push_back({a, b, c, d});
                    for (; p < q && nums[p] == c; ++p) {}
                    for (; p < q && nums[q] == d; --q) {}
                }
            }
        }
        return result;
    }
};
