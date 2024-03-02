// https://leetcode.com/problems/single-number-iii/description/

#include <vector>


class Solution {
public:
    std::vector<int> singleNumber(std::vector<int>& nums) {
        // axorb == 0 only if a == b
        // the required values are not equal

        int axorb = 0;
        for (const auto i : nums) {
            axorb ^= i;
        }

        // set bit is the bit that must be set in a but not in b
        int setBit = 1;
        for (; (axorb & setBit) == 0; setBit <<= 1) {
            // pass
        }

        // all doubled elemets will be xored to 0 since
        // a ^ b = b ^ a
        // a ^ ( b ^ c ) = ( a ^ b ) ^ c;
        int aVal = 0;
        for (const auto i : nums) {
            if (i & setBit) {
                aVal ^= i;
            }
        }

        return {aVal, axorb ^ aVal};
    }
};
