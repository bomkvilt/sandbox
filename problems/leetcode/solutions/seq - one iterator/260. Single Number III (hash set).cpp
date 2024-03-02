// https://leetcode.com/problems/single-number-iii/description/

#include <vector>
#include <unordered_set>


class Solution {
public:
    std::vector<int> singleNumber(std::vector<int>& nums) {
        std::unordered_set<int> hits;
        for (const auto num : nums) {
            auto pos = hits.find(num);
            if (pos != hits.end()) {
                hits.erase(pos);
            }
            else {
                hits.insert(num);
            }
        }
        
        std::vector<int> result;
        std::copy(hits.begin(), hits.end(), std::back_inserter(result));
        return result;
    }
};
