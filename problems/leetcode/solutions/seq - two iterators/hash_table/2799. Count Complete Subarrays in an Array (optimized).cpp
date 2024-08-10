// https://leetcode.com/problems/count-complete-subarrays-in-an-array

#include <unordered_map>
#include <vector>


// .prefix
// |   .core
// |   |     .suffix : increases result by 1 and |prefix|
// vvv vvvvv vvvvv
//    -------     -------
// * *|* * *|* * *|* * *|* * * * * *
//    |^ ^ ^|     |^ ^ ^|
//   ^|^ ^ ^|^   ^|^ ^ ^|
// ^ ^|^ ^ ^|^ ^ ^|^ ^ ^|
// -------------
//         2 2 2 2 3    prefix
//         3 6 9 1 1    result[part]
//               2 6    result[part]
//     ^           ^ here we assume that the element are the same

class Solution {
public:
    int countCompleteSubarrays(const std::vector<int>& nums) {
        std::unordered_map<int, int> window;
        for (const auto n : nums) {
            window[n] += 1;
        }

        const auto power = window.size();
        window.clear();

        int result = 0;
        int prefix = 0;

        auto i = nums.begin();
        auto j = nums.begin();
        for (; j != nums.end(); ++j) {
            window[*j] += 1;

            // NOTE: keep the window minimal
            for (; window[*i] > 1; ++i) {
                window[*i] -= 1;
                prefix += 1;
            }

            if (window.size() == power) {
                result += prefix;
                result += 1;
            }
        }

        return result;
    }
};
