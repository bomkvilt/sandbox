// https://leetcode.com/problems/permutations-ii/description/

#include <algorithm>
#include <cmath>
#include <vector>


class Solution {
public:
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        std::vector<std::vector<int>> result;
        result.reserve(std::tgamma(nums.size() + 1));

        std::ranges::sort(nums);

        do {
            result.push_back(nums);
        }
        while (std::next_permutation(nums.begin(), nums.end()));

        return result;
    }
};
