// https://leetcode.com/problems/reverse-words-in-a-string-iii/description/

#include <algorithm>
#include <string>


class Solution {
public:
    std::string reverseWords(std::string s) {
        auto a = s.begin();

        do {
            auto b = std::find(a, s.end(), ' ');
            std::reverse(a, b);

            if (b == s.end()) {
                break;
            }

            a = ++b;
        }
        while (true);

        return s;
    }
};
