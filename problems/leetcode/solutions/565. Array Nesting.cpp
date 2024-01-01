#include <unordered_set>
#include <cmath>



class Solution 
{
public:
    
    int arrayNesting(vector<int>& nums) 
    {
        auto visited = std::unordered_set<int>();
        visited.reserve(2 * nums.size());
        
        int maxLen = 0;
        for (auto num : nums)
        {
            int len = 0;
            // check if the node is not a part of an already visited chain
            // or a part of a current chain that has already been visited on the cycle
            while (visited.find(num) == visited.end())
            {
                visited.insert(num);
                ++len;
                
                num = nums[num];
            }
            maxLen = std::max(maxLen, len);
        }
        return maxLen;
    }
};
