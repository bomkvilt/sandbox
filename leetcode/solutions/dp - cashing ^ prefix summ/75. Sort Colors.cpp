class Solution {
public:
    
    void sortColors(vector<int>& nums) 
    {
        auto vals = std::array<int, 3>{0};
        
        for (const auto& n : nums)
        {
            ++vals[n];
        }
        
        auto itr = nums.begin();
        for (int i = 0; i < vals.size(); ++i)
        {
            while (vals[i]-- > 0)
            {
                *(itr++) = i;
            }
        }
    }
};
