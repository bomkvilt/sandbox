class Solution 
{
public:
    
    vector<vector<int>> generate(int numRows) 
    {
        auto ans = vector<vector<int>>(numRows);
        
        for (int row = 1; row <= numRows; ++row)
        {
            ans[row - 1].resize(row);
            
            for (int j = 0; j < row; ++j)
            {
                if (j == 0 || j == row - 1)
                {
                    ans[row - 1][j] = 1;
                    continue;
                }
                
                ans[row - 1][j] = ans[row - 2][j - 1] + ans[row - 2][j - 0];
            }
        }
        return ans;
    }
};
