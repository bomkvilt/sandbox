// https://leetcode.com/problems/next-permutation/description/

#include <algorithm>
#include <utility>
#include <vector>


class Solution {
public:
    void nextPermutation(std::vector<int>& nums) {
        if (nums.size() < 2) {
            return;
        }

        static constexpr auto comp = [](const int a, const int b) -> bool {
            return a < b;
        };

        const auto last = nums.end();
        for (auto iti = last - 1; iti != nums.begin(); ) {
            auto itk = iti;
            if (!comp(*(--iti), *itk)) {
                continue;
            }

            auto itj = last;
            while (!comp(*iti, *(--itj))) {
                // pass
            }

            std::swap(*iti, *itj);
            std::reverse(itk, last);
            return;
        }

        std::ranges::reverse(nums);
    }
};
