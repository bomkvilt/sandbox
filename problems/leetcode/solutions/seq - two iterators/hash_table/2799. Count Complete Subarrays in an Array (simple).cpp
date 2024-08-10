// https://leetcode.com/problems/count-complete-subarrays-in-an-array

#include <ranges>
#include <unordered_set>
#include <vector>


class Solution {
public:
    int countCompleteSubarrays(const std::vector<int>& nums) {
        std::unordered_set<int> window;
        for (const auto n : nums) {
            window.insert(n);
        }

        const auto power = window.size();

        int result = 0;

        for (const auto i : std::views::iota(0ul, nums.size())) {
            window.clear();
            for (const auto j : std::views::iota(i, nums.size())) {
                window.insert(nums[j]);
                if (window.size() == power) {
                    result += 1;
                }
            }
        }

        return result;
    }
};
