#include <vector>
#include <algorithm>
#include <unordered_map>


class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        if (nums.size() < 3) {
            return {};
        }

        std::ranges::sort(nums);

        if (nums.front() > 0 || nums.back() < 0) {
            return {};
        }

        auto lastKeys = std::unordered_map<int, std::vector<int>::const_iterator>{};
        for (auto it = nums.cbegin(); it < nums.cend(); ++it) {
            lastKeys[*it] = it;
        }

        auto forward = [&nums](auto& it) -> void {
            for (const auto& base = *it; it != nums.end() && *(++it) == base; ) {}
        };

        auto result = std::vector<std::vector<int>>{};
        for (auto it1 = nums.begin(); it1 < nums.end() && *it1 <= 0; forward(it1)) {
            for (auto it2 = it1 + 1; it2 < nums.end(); forward(it2)) {
                const auto key = -(*it1 + *it2);
                const auto pos = lastKeys.find(key);
                if (pos != lastKeys.end() && pos->second > it2) {
                    result.push_back({*it1, *it2, key});
                }
            }
        }
        
        return result;
    }
};
