class Solution 
{
public:
    
    vector<int> sortedSquares(const vector<int>& nums) 
    {
        const auto pivot = GetPivot(nums);
        if (pivot == -1)
        {
            return {};
        }
        
        auto squares = vector<int>();
        squares.reserve(nums.size());
        
        int a = pivot;
        int b = pivot + 1;
        while (a >= 0 || b < nums.size())
        {
            if (a < 0)
            {
                const auto B = nums[b++];
                squares.push_back(B*B);
                continue;
            }
            if (b >= nums.size())
            {
                const auto A = nums[a--];
                squares.push_back(A*A);
                continue;
            }
            
            const auto A = nums[a];
            const auto B = nums[b];
            if (-A < B)
            {
                squares.push_back(A*A);
                --a;
            }
            else
            {
                squares.push_back(B*B);
                ++b;
            }
        }
        return squares;
    }
    
    
    int GetPivot(const vector<int>& nums)
    {
        const auto n = nums.size();
        if (n < 1)
        {
            return -1;
        }
        if (nums[0] >= 0)
        {
            return 0;
        }
        if (nums[n-1] <= 0)
        {
            return n - 1;
        }
        
        for (int i = 0; i < n - 1; ++i)
        {
            const auto curr = nums[i];
            const auto next = nums[i + 1];
            if (curr < 0 && next >= 0)
            {
                return i;
            }
        }
        throw std::runtime_error("unexpected number case");
    }
};
