// https://leetcode.com/problems/compare-version-numbers/


#include <charconv>

class Solution {
public:
    int compareVersion(string version1, string version2) {
        constexpr static auto nextDot = [](const auto& str, size_t pos) -> size_t {
            if(auto next = str.find('.', pos + 1); next != str.npos) {
                return next;
            }
            return str.size();
        };
        constexpr static auto toInt = [](const std::string& str, size_t pos, size_t end) {
            int result = 0;
            std::from_chars(str.data() + pos, str.data() + end, result);
            return result;
        };
        constexpr static auto step = [](const std::string& str, size_t pos) -> std::pair<size_t, int> {
            if (pos == str.size()) {
                return {pos, 0};
            }
            auto next = nextDot(str, pos);
            auto value = toInt(str, pos, next);
            return {next != str.size() ? next + 1 : next, value};
        };
        
        auto pos1 = 0; auto end1 = version1.size();
        auto pos2 = 0; auto end2 = version2.size();
        while (pos1 != end1 || pos2 != end2) {
            auto [next1, rev1] = step(version1, pos1);
            auto [next2, rev2] = step(version2, pos2);
            
            if (rev1 < rev2) {
                return -1;
            }
            if (rev1 > rev2) {
                return +1;
            }
            
            pos1 = next1;
            pos2 = next2;
        }
        
        return 0;
    }
};
