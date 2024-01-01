class Solution {
public:
    
    string longestPalindrome(string s) 
    {
        if (s.length() == 0)
        {
            return "";
        }
        
        // start, len
        auto stat = std::array<int, 2>{0, 0};
        
        for (int i = 0; i < s.length(); ++i)
        {
            IsPalindrome(s, i, i + 0, stat);
            IsPalindrome(s, i, i + 1, stat);
        }
        return s.substr(stat[0], stat[1]);
    }
    
private:
    
    void IsPalindrome(const std::string& s, int l, int r, std::array<int, 2>& stat)
    {
        if (l > r)
        {
            return;
        }
        
        while (l >= 0 && r < s.length() && s[l] == s[r])
        {
            --l;
            ++r;
        }
        
        const auto len = r - l - 1;
        if (len > stat[1])
        {
            stat[0] = l+1;
            stat[1] = len;
        }
    }
};
