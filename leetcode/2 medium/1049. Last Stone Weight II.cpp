#include <vector>
#include <numeric>
#include <algorithm>



class Solution 
{
public:
    int lastStoneWeightII(const vector<int>& stones) 
    {
        const auto S = std::accumulate(stones.begin(), stones.end(), 0);
        const auto W = S / 2;
        
        auto m = std::vector<int>(W + 1, 0);
        
        for (auto wj : stones)
        {
            for (int w = W; w >= wj; --w)
            {
                m[w] = std::max(m[w], m[w - wj] + wj);
            }
        }
        return S - 2 * m[W];
    }
};
