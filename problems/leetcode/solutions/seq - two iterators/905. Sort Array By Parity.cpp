// https://leetcode.com/problems/sort-array-by-parity/


class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& nums) {
        if (!nums.size()) {
            return {};
        }

        size_t i = 0;
        size_t j = nums.size() - 1;
        
        for (; i < j; ) {
            if ((nums[i] & 1) == 0) {
                ++i;
            }
            else if ((nums[j] & 1) == 1) {
                --j;
            }
            else {
                std::swap(nums[i], nums[j]);
            }
        }
        return std::move(nums);
    }
};
