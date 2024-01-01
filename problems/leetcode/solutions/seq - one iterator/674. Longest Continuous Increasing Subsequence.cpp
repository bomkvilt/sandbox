class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        if (!nums.size()) {
            return 0;
        }

        size_t max_len = 0;
        size_t len = 1;
        const auto flush = [&]() {
            max_len = std::max(max_len, len);
            len = 1;
        };

        for (size_t i = 1; i < nums.size(); ++i) {
            if (nums[i] > nums[i - 1]) {
                len += 1;
                continue;
            }
            flush();
        }

        flush();

        return max_len;
    }
};
