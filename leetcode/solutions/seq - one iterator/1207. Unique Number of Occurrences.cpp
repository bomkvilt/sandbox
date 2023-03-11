// https://leetcode.com/problems/unique-number-of-occurrences/


class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        auto hits = std::unordered_map<int, int>{};
        for (const int v : arr) {
            hits[v] += 1;
        }
        
        auto hit_counts = std::unordered_set<int>{};
        for (const auto& [key, count] : hits) {
            if (hit_counts.find(count) != hit_counts.end()) {
                return false;
            }
            hit_counts.insert(count);
        }
        
        return true;
    }
};
