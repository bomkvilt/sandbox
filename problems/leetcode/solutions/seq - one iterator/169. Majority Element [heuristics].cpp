// https://leetcode.com/problems/majority-element/description/

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int count = 0;
        int target = nums[0];
        for (const auto num : nums) {
            if (!count) {
                target = num;
            }
            count += 1 - 2 * int(target != num);
        }
        return target;
    }
};
