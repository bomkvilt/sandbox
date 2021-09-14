#include <unordered_map>


class Solution 
{
public:
    string customSortString(const string& order, const string& s) 
    {
        auto charCounts = unordered_map<char, int>();
        
        auto extractCharCount = [&charCounts](const char c) -> int
        {
            auto itr = charCounts.find(c);
            if (itr == charCounts.end())
            {
                return -1;
            }
            
            const auto count = itr->second;
            charCounts.erase(c);
            return count;
        };
        
        for (auto c : s)
        {
            ++charCounts[c];
        }
        
        auto outstring = std::string();
        outstring.reserve(s.length() + 1);
        
        for (auto c : order)
        {
            const auto count = extractCharCount(c);
            if (count < 0)
            {
                continue;
            }
            
            for (int i = 0; i < count; ++i)
            {
                outstring += c;
            }
        }
        
        for (const auto& [c, count] : charCounts)
        {
            for (int i = 0; i < count; ++i)
            {
                outstring += c;
            }
        }
        
        return outstring;
    }
};
