class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        if (!nums.size()) {
            return {};
        }

        vector<int> result(nums.size(), 0);

        int suffix = 1;
        size_t i = nums.size() - 1;
        for (; i > 0 && suffix; --i) {
            suffix *= nums[i];
            result[i] = suffix;
        }

        int prefix = 1;
        for (size_t j = 0; j < i && prefix; ++j) {
            prefix *= nums[j];
        }

        size_t k = i;
        for (; k < nums.size() - 1; ++k) {
            result[k] = prefix * result[k + 1];
            prefix *= nums[k];
        }
        result[k] = prefix;

        return result;
    }
};