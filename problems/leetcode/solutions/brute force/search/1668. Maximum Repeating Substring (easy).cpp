// https://leetcode.com/problems/maximum-repeating-substring

#include <string>


class Solution {
public:
    // W((n + m) * n/m) = W(n**2 / m + n)
    int maxRepeating(const std::string& sequence, const std::string& word) {
        int k = 0;
        for (std::string sub = word; sequence.find(sub) != std::string::npos; sub += word) {
            k += 1;
        }
        return k;
    }
};
