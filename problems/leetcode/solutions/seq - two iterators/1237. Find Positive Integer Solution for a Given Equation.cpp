/*
 * // This is the custom function interface.
 * // You should not implement it, or speculate about its implementation
 * class CustomFunction {
 * public:
 *     // Returns f(x, y) for any given positive integers x and y.
 *     // Note that f(x, y) is increasing with respect to both x and y.
 *     // i.e. f(x, y) < f(x + 1, y), f(x, y) < f(x, y + 1)
 *     int f(int x, int y);
 * };
 */

class Solution {
public:
    vector<vector<int>> findSolution(CustomFunction& cf, int z) 
    {
        int a = FindInitialPoint(cf, z);
        int b = 1;
        
        auto ans = vector<vector<int>>();
        for (; ; ++b)
        {
            for (; a > 0 && cf.f(a, b) > z; --a)
            {
                // do nothing
            }
            if (a == 0)
            {
                // isoline is out of Q1
                break;
            }
            
            // is we are on the border
            if (cf.f(a, b) == z)
            {
                ans.push_back({a, b});
                continue;
            }
        }
        return ans;
    }
    
    // find a number a : f(a, 1) <= z
    int FindInitialPoint(CustomFunction& cf, const int z)
    {
        const int b = 1;
        
        int a = 1;
        for (; cf.f(a, b) < z; ++a)
        {
            // do nothing
        }
        
        return a;
    }
};
