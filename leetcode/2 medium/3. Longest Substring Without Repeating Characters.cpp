#include <unordered_map>



class Solution 
{
public:

    int lengthOfLongestSubstring(string s) 
    {
        auto lastCharMap = std::unordered_map<char, int>();
        
        int curLen = 0;
        int maxLen = 0;
        for (int i = 0; i < s.length(); ++i)
        {
            const auto c = s[i];
            
            auto itr = lastCharMap.find(c);
            if (itr != lastCharMap.end())
            {
                maxLen = std::max(maxLen, curLen);
                curLen = std::min(curLen + 1, i - itr->second);
            }
            else
            {
                ++curLen;
            }
            lastCharMap[c] = i;
        }
        return std::max(curLen, maxLen);
    }
};
