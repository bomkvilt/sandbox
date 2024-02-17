#include <string>
#include <unordered_map>


class Solution {
public:
    int minSteps(const std::string& s, const std::string& t) {
        std::unordered_map<char, int> ballance;
        
        for (const auto c : s) {
            ballance[c] += 1;
        }

        for (const auto c : t) {
            ballance[c] -= 1;
        }

        int diff = 0;
        for (const auto [_, v] : ballance) {
            diff += std::abs(v);
        }
        
        return diff / 2;
    }
};
