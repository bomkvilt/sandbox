#include <unordered_map>



class Solution 
{
public:

    vector<int> twoSum(const vector<int>& nums, int target) 
    {
        auto numCash = std::unordered_multimap<int, int>();
        for (int i = 0; i < nums.size(); ++i)
        {
            numCash.insert({nums[i], i});
        }
        
        for (int i = 0; i < nums.size(); ++i)
        {
            const auto num  = nums[i];
            const auto left = target - num;
            
            auto range = numCash.equal_range(left);
            for (auto itr = range.first; itr != range.second; ++itr)
            {
                if (itr->second == i)
                {
                    continue;
                }
                return {i, itr->second};
            }
        }
        return {};
    }
};
