#include <unordered_map>
#include <algorithm>


class Solution 
{
private:
    
    constexpr static auto MOD = 1000000007;
    
    using Cash = std::unordered_map<int, std::unordered_map<int, int>>;
    
public:
    
    int numRollsToTarget(int d, int f, int target) 
    {
        auto cash = Cash();
        return solve(d, f, target, cash);
    }
    
private:
    
    int solve(int d, int f, int target, Cash& cash)
    {
        if (d * f < target || d > target)
        {
            return 0;
        }
        
        if (d == 1)
        {
            return (f >= target) ? 1 : 0;
        }
        
        auto& lineCash = cash[d];
        auto itr = lineCash.find(target);
        auto end = lineCash.end();
        if (itr != end)
        {
            return itr->second;
        }
        
        int64_t sum = 0;
        
        int max = std::min(f, target);
        for (int i = 1; i <= max; ++i)
        {
            sum += solve(d - 1, f, target - i, cash);
            sum %= MOD;
        }
        return cash[d][target] = sum;
    }
    
};
