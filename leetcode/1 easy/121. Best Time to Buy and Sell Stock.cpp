#include <unordered_map>
#include <algorithm>



class Solution 
{
public:

    int maxProfit(const vector<int>& prices) 
    {
        const auto n = prices.size();        
        if (n < 1)
        {
            return 0;
        }
        
        auto leftMax = std::unordered_map<int, int>();
        leftMax[n-1] = prices[n-1];
        
        for (int i = n - 2; i > 0; --i)
        {
            leftMax[i] = std::max(prices[i], leftMax[i + 1]);
        }
        
        int maxProfit = 0;
        for (int i = 0; i < n - 1; ++i)
        {
            const auto a = prices [i];
            const auto b = leftMax[i + 1];
            const auto profit = b - a;
            
            if (profit > 0)
            {
                maxProfit = std::max(profit, maxProfit);
            }
        }
        return maxProfit;
    }
};
