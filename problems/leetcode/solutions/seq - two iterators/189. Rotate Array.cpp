// https://leetcode.com/problems/rotate-array/

#include <algorithm>
#include <vector>


class Solution {
public:
    void rotate(std::vector<int>& nums, int k) {
        k = k % nums.size();
        std::reverse(nums.begin(), nums.end() - k);
        std::reverse(nums.end() - k, nums.end());
        std::ranges::reverse(nums);
    }
};
