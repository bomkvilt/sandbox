#pragma GCC optimize("O3")

static auto _f = [](){
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return nullptr;
}();


#include <functional>
#include <algorithm>
#include <queue>
#include <vector>



class Solution 
{
public:
    int minDifference(const vector<int>& nums) 
    {
        if (nums.size() < 4)
        {
            return 0;
        }
        
        auto maxes = std::priority_queue<int, std::vector<int>, std::greater<int>>();
        auto mins  = std::priority_queue<int>();
        
        for (auto num : nums)
        {
            maxes.push(num);            
            if (maxes.size() > 4)
            {
                maxes.pop();
            }
            
            mins.push(num);            
            if (mins.size() > 4)
            {
                mins.pop();
            }
        }
        
        const auto n = nums.size();
        const auto m = mins.size();
        const auto w = std::min(2*m, n);
        
        auto window = std::vector<int>(w, 0);
        for (int i = m - 1; i >= 0; --i)
        {
            window[i] = mins.top();
            mins.pop();
        }
        for (int i = w - m; i < w; ++i)
        {
            window[i] = maxes.top();
            maxes.pop();
        }
        
        
        int min = INT_MAX;
        const auto s = w - 3;
        for (int i = 0; i < 4; ++i)
        {
            const auto delta = window[s - 1 + i] - window[i];
            min = std::min(min, delta);
        }
        return min;
    }
};
