// https://leetcode.com/problems/4sum-ii/description/


class Solution {
public:
    int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
        auto summHits = std::unordered_map<int, int>{};
        for (const auto& a : nums1)
        for (const auto& b : nums2) {
            const auto summ = a + b;
            summHits[summ] += 1;
        }

        static const int TARGET = 0;

        int result = 0;
        for (const auto& c : nums3)
        for (const auto& d : nums4) {
            const auto r = TARGET - c - d;

            const auto pos = summHits.find(r);
            if (pos != summHits.end()) {
                result += pos->second;
            }
        }

        return result;
    }
};
