class Solution 
{
public:
    
    string removeKdigits(const std::string& num, int k) 
    {
        if (num.size() <= k)
        {
            return "0";
        }
        
        // make smallest string
        auto ans = std::string();
        ans.reserve(num.size());
        
        for (auto c : num)
        {
            for (; ans.size() && k > 0 && c < ans.back(); --k)
            {
                ans.resize(ans.size() - 1);
            }
            ans += c;
        }
        for (; k > 0; --k)
        {
            ans.resize(ans.size() - 1);
        }
        
        // remove leading zeros
        auto i = ans.begin();
        auto j = ans.begin();
        auto end = ans.end();
        for (; j != end && *j == '0'; ++j)
        {
            // do nothing
        }
        ans.erase(i, j);
        
        return ans.size() ? ans : "0";
    }
};
