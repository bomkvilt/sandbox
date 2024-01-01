// https://leetcode.com/problems/count-number-of-teams/

class Solution {
public:
    int numTeams(vector<int>& rating) {
        return
            Inner(rating, [](int ri, int rj) { return ri > rj; }) +
            Inner(rating, [](int ri, int rj) { return ri < rj; })
        ;
    }

private:
    template<typename TOrder>
    int Inner(const vector<int>& rating, TOrder&& order) {
        int result = 0;
        
        auto hits = std::vector<int>(rating.size(), 0);
        for (int i = 0 + 1; i < rating.size(); ++i)
        for (int j = i - 1; j >= 0; --j) {
            if (!order(rating[j], rating[i])) {
                continue;
            }
            
            hits[i] += 1;
            result += hits[j];
        }
        
        return result;
    }
};
