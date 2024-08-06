// https://leetcode.com/problems/di-string-match

#include <string>
#include <vector>


class Solution {
public:
    std::vector<int> diStringMatch(const std::string& s) {
        std::vector<int> result;
        result.reserve(s.size() + 1);

        int b = 0;
        int e = s.size();

        for (const auto c : s) {
            result.push_back(c == 'I' ? b++ : e--);
        }

        if (b == e) {
            result.push_back(b);
        }

        return result;
    }
};
