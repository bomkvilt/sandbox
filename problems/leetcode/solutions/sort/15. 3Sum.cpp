#include <vector>
#include <unordered_map>
#include <algorithm>


class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());

        if (nums.size() < 3) {
            return {};
        }
        if (nums[0] > 0) {
            return {};
        }

        std::unordered_map<int, size_t> lastKeys;
        for (size_t i = 0; i < nums.size(); ++i) {
            lastKeys[nums[i]] = i;
        }

        std::vector<std::vector<int>> result;
        for (size_t i = 0; i < nums.size() - 2; ++i) {
            if (i != 0 && nums[i] == nums[i - 1]) {
                continue;
            }
            if (nums[i] > 0) {
                break;
            }
            
            const auto summ = nums[i];
            for (size_t j = i + 1; j < nums.size() - 1; ++j) {
                if (j != i + 1 && nums[j] == nums[j - 1]) {
                    continue;
                }

                const auto key = -summ - nums[j];
                const auto pos = lastKeys.find(key);
                if (pos != lastKeys.end() && pos->second > j) {
                    result.push_back({summ, nums[j], key});
                }
            }
        }

        return result;
    }
};
