class Solution 
{
public:

    int hammingWeight(uint32_t n) 
    {
        int count = 0;
        for (int i = 0; i < sizeof(n) * 8; ++i, n >>= 1)
        {
            count += n & 1;
        }
        return count;
    }
};
