#include <vector>
#include <algorithm>



class Solution 
{
public:
    vector<vector<int>> diagonalSort(vector<vector<int>> mat) 
    {
        const auto h = mat.size();
        const auto w = mat[0].size();
        const auto d = std::min(h, w);
        
        auto diag = std::vector<int>();
        diag.reserve(d);
        
        for (int i = 0; i < w; ++i)
        {
            diag.clear();
            
            const auto J = std::min(w - i, h);
            
            for (int j = 0; j < J; ++j)
            {
                diag.push_back(mat[j][j + i]);
            }
            
            std::sort(diag.begin(), diag.end());
            
            for (int j = 0, p = 0; j < J; ++j, ++p)
            {
                mat[j][j + i] = diag[p];
            }
        }
        
        for (int i = 0; i < h; ++i)
        {
            diag.clear();
            
            const auto J = std::min(w, h - i);
            
            for (int j = 0; j < J; ++j)
            {
                diag.push_back(mat[j + i][j]);
            }
            
            std::sort(diag.begin(), diag.end());
            
            for (int j = 0, p = 0; j < J; ++j, ++p)
            {
                mat[j + i][j] = diag[p];
            }
        }
        
        return mat;
    }
};
