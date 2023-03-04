class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        assert(nums.size() && (nums.size() & 1));

        size_t l = 0;
        size_t r = nums.size();
        while (l + 1 < r) {
            const auto m = l + (r - l) / 2;
            if (nums[m] == nums[m + 1]) {
                if (m & 1) r = m;
                else l = m + 2;
            }
            else if (nums[m] == nums[m - 1]) {
                if (m & 1) l = m + 1;
                else r = m - 1;
            }
            else {
                l = m;
                break;
            }
        }
        return nums[l];
    }
};

// {2i} == {2i + 1}
// -> [0, 2i + 1] -> correct

// {2i} == {2i - 1}
// -> [2i - 1, N) -> correct
