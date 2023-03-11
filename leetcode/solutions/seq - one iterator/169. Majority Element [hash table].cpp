// https://leetcode.com/problems/majority-element/description/

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        auto stats = std::unordered_map<int, int>();
        for (const auto num : nums) {
            stats[num] += 1;
        }
        
        const auto threshold = nums.size() / 2;
        for (const auto& [num, hits] : stats) {
            if (hits > threshold) {
                return num;
            }
        }
        assert(false);
    }
};
