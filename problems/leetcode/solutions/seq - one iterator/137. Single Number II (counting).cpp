// https://leetcode.com/problems/single-number-ii/description/

#include <vector>

// 1 1 1 2 2 2[5] | 5 5 5 6 6 6[7]
// 0000 0001      | 0000 0100
// 0000 0001      | 0000 0100
// 0000 0001      | 0000 0100
// 0000 0010      | 0000 0101
// 0000 0010      | 0000 0101
// 0000 0010      | 0000 0101
// 0000 0100      | 0000 0111
// |||| |||| sum  | |||| |||| sum
// 0000 0133      | 0000 0714
// |||| |||| % 3  | |||| |||| % 3
// 0000 0100 == 7 | 0000 0111 == 7

class Solution {
public:
    // can be vectorized https://godbolt.org/z/Pf8Gbc89z
    // sa: manual vectorisation @ {137. Single Number II (xor).cpp}
    int singleNumber(const std::vector<int>& nums) {
        int a = 0;
        for (int i = 0; i < 32; ++i) {
            int sum = 0;
            for (const auto num : nums) {
                sum += (num >> i) & 1;
            }
            a |= (sum % 3) << i;
        }
        return a;
    }
};
