// https://leetcode.com/problems/detect-pattern-of-length-m-repeated-k-or-more-times/

#include <vector>


class Solution {
public:
    bool containsPattern(const std::vector<int>& arr, const int m, const int k) {
        const size_t batchSize = m * (k - 1); // NOTE: look ahead
        const size_t lastIdx = arr.size() - m;

        size_t hits = 0;
        for (size_t i = 0; i < lastIdx && hits < batchSize; ++i) {
            static constexpr auto ZEROS = static_cast<size_t>(0);
            hits = (hits + 1) & (ZEROS - (arr[i] == arr[i + m]));
        }

        return hits >= batchSize;
    }
};
