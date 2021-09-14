class Solution 
{
public:
    
    vector<int> productExceptSelf(const vector<int>& nums) 
    {
        int64_t product = 1;
        int zeros = 0;
        
        for (auto num : nums)
        {
            if (num)
            {
                product *= num;
            }
            else
            {
                ++zeros;
            }
        }
        
        auto answer = vector<int>();
        answer.reserve(nums.size());
        
        for (auto num : nums)
        {
            if (zeros == 0)
            {
                answer.push_back(product / num);
            }
            else if (num == 0 && zeros == 1)
            {
                answer.push_back(product);
            }
            else
            {
                answer.push_back(0);
            }
        }
        return answer;
    }
};
