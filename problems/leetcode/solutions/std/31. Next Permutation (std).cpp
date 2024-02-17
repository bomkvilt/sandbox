// https://leetcode.com/problems/next-permutation/description/

#include <algorithm>
#include <vector>


class Solution {
public:
    void nextPermutation(std::vector<int>& nums) {
        std::next_permutation(nums.begin(), nums.end());
    }
};
