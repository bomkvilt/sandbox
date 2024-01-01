// https://leetcode.com/problems/to-lower-case/


class Solution {
public:
    string toLowerCase(string s) {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c){ return std::tolower(c); }
        );
        return s;
    }
};
