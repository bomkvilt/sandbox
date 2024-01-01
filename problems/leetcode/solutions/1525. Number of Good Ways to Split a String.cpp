#include <unordered_map>
#include <vector>



class Solution 
{
public:
    int numSplits(const string& s) 
    {
        if (s.size() < 2)
        {
            return 0;
        }
        
        auto substrUniqs = std::vector<int>(s.size(), 0);
        auto charset     = std::unordered_map<char, bool>();
        
        for (int i = s.size() - 1; i > 0; --i)
        {
            charset[s[i]] = true;
            substrUniqs[i] = charset.size();
        }
        
        charset.clear();
        
        int counter = 0;
        for (int i = 0; i < s.size() - 1; ++i)
        {
            charset[s[i]] = true;
            
            const auto kl = charset.size();
            const auto kr = substrUniqs[i + 1];
            if (kl == kr)
            {
                ++counter;
            }
            else if (kl > kr)
            {
                break;
            }                
        }
        return counter;
    }
};
