// https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        auto [its, ite] = std::equal_range(nums.begin(), nums.end(), target);
        if (its != nums.end() && *its == target) {
            return {
                (int)std::distance(nums.begin(), its),
                (int)std::distance(nums.begin(), std::prev(ite)),
            };
        }
        
        return {-1, -1};
    }
};
