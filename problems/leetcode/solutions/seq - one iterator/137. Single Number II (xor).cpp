// https://leetcode.com/problems/single-number-ii/description/

#include <vector>

// dx 0 1[0]1 1
// x1 0 1 1 0 0
// x2 0 0 0 1 0

// 0: x1 = 0 ^ (0 & 1) = 0
//    x2 = 0 ^ (0 & 1) = 0
// 1: x1 = 0 ^ (1 & 1) = 1
//    x2 = 0 ^ (1 & 0) = 0
// 1: x1 = 1 ^ (0 & 1) = 1
//    x2 = 0 ^ (0 & 0) = 0
// 2: x1 = 1 ^ (1 & 1) = 0
//    x2 = 0 ^ (1 & 0) = 1
// 3: x1 = 0 ^ (1 & 0) = 0
//    x2 = 1 ^ (1 & 1) = 0

class Solution {
public:
    int singleNumber(const std::vector<int>& nums) {
        int x1 = 0;
        int x2 = 0;

        // de facto manual vectorisation of
        // 137. Single Number II (count).cpp
        for (const int num : nums) {
            x1 ^= num & ~x2;  // remove bits that where used twice
            x2 ^= num & ~x1;  // remove bits that where used once
        }
        return x1;
    }
};
