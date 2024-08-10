// https://leetcode.com/problems/subarrays-with-k-different-integers

#include <unordered_map>
#include <vector>

/**
 * @note: the same as `2799. Count Complete Subarrays in an Array (optimized)`
 */
class Solution {
public:
    int subarraysWithKDistinct(const std::vector<int>& nums, int k) {
        std::unordered_map<int, int> window;

        int result = 0;
        int prefix = 0;

        auto i = nums.begin();
        auto j = nums.begin();

        for (; j != nums.end(); ++j) {
            window[*j] += 1;

            // new element, the prefix must be invalidated
            for (; window.size() > k; ++i) {
                if ((window[*i] -= 1) == 0) {
                    window.erase(*i);
                }
                prefix = 0;
            }

            for (; window[*i] > 1; ++i) {
                window[*i] -= 1;
                prefix += 1;
            }

            if (window.size() == k) {
                result += prefix;
                result += 1;
            }
        }

        return result;
    }
};
