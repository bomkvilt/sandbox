class Solution 
{
public:

    vector<int> plusOne(vector<int>& digits) 
    {
        auto shift = 1;
        auto itr = digits.rbegin();
        auto end = digits.rend();
        for (; itr < end; ++itr)
        {
            assert(shift == 0 || shift == 1);
            if (*itr < 9)
            {
                *itr += shift;
                shift = 0;
                break;
            }
            *itr  = 0;
            shift = 1;
        }
        
        if (shift > 0)
        {
            digits.insert(digits.begin(), shift);
        }
        return digits;
    }
};
