#include <algorithm>
#include <cstdlib>
#include <vector>


class Solution {
public:
    int threeSumClosest(std::vector<int>& nums, int target) {
        if (nums.size() < 3) {
            return 0;
        }

        std::sort(nums.begin(), nums.end());

        int result = INT_MAX;
        int minDistance = INT_MAX;

        for (size_t i = 0; i < nums.size() - 2; ++i) {
            for (int j = i + 1, k = nums.size() - 1; j < k; ) {
                const auto summ = nums[i] + nums[j] + nums[k];

                const auto currentDistance = std::abs(target - summ);
                if (currentDistance < minDistance) {
                    minDistance = currentDistance;
                    result = summ;
                }

                if (summ > target) {
                    --k;
                }
                else if (summ < target) {
                    ++j;
                }
                else {
                    return summ;
                }
            }
        }

        return result;
    }
};
