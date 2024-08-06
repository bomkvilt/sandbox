// https://leetcode.com/problems/minimum-number-of-operations-to-make-word-k-periodic

#include <algorithm>
#include <ranges>
#include <string_view>
#include <unordered_map>


class Solution {
public:
    int minimumOperationsToMakeKPeriodic(const std::string_view word, const int k) {
        std::unordered_map<std::string_view, size_t> hits;

        for(size_t i = 0; i <= word.size() - k; i += k) {
            hits[word.substr(i, k)] += 1;
        }

        if (hits.empty()) {
            return 0;
        }

        const auto maxPos = std::ranges::max_element(std::views::values(hits));
        return word.size() / k - *maxPos;
    }
};
