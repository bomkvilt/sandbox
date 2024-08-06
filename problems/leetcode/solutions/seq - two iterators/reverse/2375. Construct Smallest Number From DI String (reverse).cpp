// https://leetcode.com/problems/construct-smallest-number-from-di-string/

#include <algorithm>
#include <string>
#include <ranges>


class Solution {
public:
    std::string smallestNumber(const std::string& pattern) {
        std::string result;
        result.reserve(pattern.size() + 1);

        for (const auto i : std::views::iota(0ul, pattern.size() + 1)) {
            result.push_back('1' + i);
        }

        for (size_t j = 0; j < pattern.size(); ++j) {
            if (pattern[j] == 'I') {
                continue;
            }

            auto i = j;
            for (; j != pattern.size() && pattern[j] == 'D'; ++j) {}

            // NOTE: |result| = |pattern| + 1
            std::reverse(result.begin() + i, result.begin() + j + 1);
        }

        return result;
    }
};
