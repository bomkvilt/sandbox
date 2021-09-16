#include <array>
#include <algorithm>



class Solution {
public:
    int minFallingPathSum(const vector<vector<int>>& matrix) 
    {
        const auto n = matrix.size();
        auto currsums = std::array<vector<int>, 2>();
        auto currturn = 0;
        auto prevturn = 1;
        
        if (n < 2)
        {
            return matrix[0][0];
        }
        
        auto newTurn = [&currturn, &prevturn]() 
        {
            std::swap(prevturn, currturn);
        };
        
        currsums[currturn] = matrix[n-1];
        currsums[prevturn] = vector<int>(n, 0);
        newTurn();
        
        
        int absMin = 0;
        for (int row = n-2; row >= 0; --row, newTurn())
        for (int col = 0  ; col <  n; ++col)
        {
            int val = matrix[row][col];
            int min = currsums[prevturn][col];
            
            if (col > 0)
            {
                min = std::min(min, currsums[prevturn][col - 1]);
            }
            if (col < n - 1)
            {
                min = std::min(min, currsums[prevturn][col + 1]);
            }
            
            const int sum = val + min;
            currsums[currturn][col] = sum;
            
            if (row != 0)
            {
                continue;
            }
            absMin = col > 0 
                ? std::min(absMin, sum)
                : sum;
        }
        return absMin;
    }
};
