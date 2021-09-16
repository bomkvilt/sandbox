#include <unordered_map>



class Solution 
{
public:
    
    bool buddyStrings(const string& s, const string& goal) 
    {
        if (s.length() != goal.length())
        {
            return false;
        }
        
        int  count  = 0;
        char letter0 = 0;
        char letter1 = 0;
        bool bRepeats = false;
        auto charCounters = std::unordered_map<char, int>();
        for (int i = 0; i < s.length(); ++i)
        {
            if (!bRepeats)
            {
                bRepeats |= ++charCounters[s[i]] >= 2;
            }            
            if (s[i] == goal[i])
            {
                continue;
            }
            
            switch(++count)
            {
            case 1:
                letter0 = s[i];
                letter1 = goal[i];
                break;
                
            case 2:
                if (goal[i] != letter0 || s[i] != letter1)
                {
                    return false;
                }
                break;
            
            default: return false;
            }
        }
        return count == 2 || count == 0 && bRepeats;
    }
};
