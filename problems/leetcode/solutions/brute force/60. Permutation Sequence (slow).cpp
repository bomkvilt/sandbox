// https://leetcode.com/problems/permutation-sequence/description/

#include <algorithm>
#include <string>


class Solution {
public:
    std::string getPermutation(int n, int k) {
        std::string nums;
        nums.reserve(n);

        for (char i = 0; i < n; ++i) {
            nums += '1' + i;
        }

        for (int i = 1; i < k; ++i) {
            std::next_permutation(nums.begin(), nums.end());
        }
        return nums;
    }
};
