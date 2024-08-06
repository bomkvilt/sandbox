// https://leetcode.com/problems/construct-smallest-number-from-di-string/

#include <string>


class Solution {
public:
    std::string smallestNumber(const std::string& pattern) {
        std::string result;
        result.reserve(pattern.size() + 1);

        auto moveD = [&](auto pos, const auto end) -> auto {
            for (; pos != end && *pos == 'D'; ++pos) {}
            return pos;
        };

        int offset = 0;
        for (auto pos = pattern.begin(); pos != pattern.end(); ) {
            const auto next = moveD(pos, pattern.end());

            if (pos == next) {
                result.push_back('1' + offset);
                offset = result.size();
                ++pos;
                continue;
            }

            offset = result.size();
            auto num = '1' + offset + std::distance(pos, next);

            for (; pos != next; ++pos, --num) {
                result.push_back(num);
            }
        }

        result.push_back('1' + offset);
        return result;
    }
};
