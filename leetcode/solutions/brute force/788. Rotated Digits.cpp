#include <algorithm>
#include <vector>



class Solution 
{
public:
    
    int rotatedDigits(int n) 
    {
        int count = 0;
        for (int i = 2; i <= n; ++i)
        {
            count += solve(i);
        }
        return count;
    }
    
private:
    
    int solve(int n)
    {
        bool bFound = false;
        for (; n; n /= 10)
        {
            switch (n % 10) 
            {
            case 3: case 4: case 7: 
                return 0;
            
            case 2: case 5: case 6: case 9:
                bFound = true;
            }
        }
        return  bFound;
    }
};
