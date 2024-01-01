// https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

#include <unordered_set>


class Solution {
public:
    int lengthOfLongestSubstring(std::string s) {
        if (s.empty()) {
            return 0;
        }

        std::unordered_set<char> chars;
        
        size_t maxWidth = 0;

        auto l = s.begin();
        auto r = s.begin();
        for (; r != s.end(); ++r) {
            const auto ci = *r;
            while (chars.find(ci) != chars.end()) {
                chars.erase(*(l++));
            }
            chars.insert(ci);
            maxWidth = std::max(maxWidth, chars.size());
        }
        
        return maxWidth;
    }
};
