// https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/


class Solution {
public:
    int strStr(string haystack, string needle) {
        const auto idx = haystack.find(needle);
        return idx != std::string::npos ? idx : -1;
    }
};
