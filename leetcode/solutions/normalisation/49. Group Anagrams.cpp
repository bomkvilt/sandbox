// https://leetcode.com/problems/group-anagrams/description/

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        auto groups = std::unordered_map<std::string, std::vector<string>>();
        for (auto& s : strs) {
            groups[MakeANorm(s)].emplace_back(std::move(s));
        }
        
        auto result = std::vector<std::vector<std::string>>();
        for (auto&& [_, values] : groups) {
            result.emplace_back(std::move(values));
        }
        return result;
    }
    
private:
    std::string MakeANorm(const std::string& base) {
        auto norm = base;
        std::sort(norm.begin(), norm.end());
        return norm;
    }
};
