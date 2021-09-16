#include <algorithm>



class Solution 
{
public:

    string reverseStr(string s, const int k) 
    {
        const int slen = s.length();
        const int step = 2*k;
        
        for (int i = 0; i < slen; i += step)
        {
            const int J = std::min(slen, i + k);
            const int M = i + (J - i) / 2;
            for (int j0 = i, j1 = J-1; j0 < M; ++j0, --j1)
            {
                std::swap(s[j0], s[j1]);
            }
        }
        return s;
    }
};
