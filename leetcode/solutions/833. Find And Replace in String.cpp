#include <unordered_map>
#include <unordered_set>



class Solution 
{
public:
    string findReplaceString(string s
        , vector<int   >& indices
        , vector<string>& sources
        , vector<string>& targets
    ) 
    {
        auto drops    = std::unordered_set<int>();
        auto indjects = std::unordered_map<int, std::string>();
        
        int ballance = 0;
        for (int i = 0; i < indices.size(); ++i)
        {
            const auto  pos = indices[i];
            const auto& src = sources[i];
            const auto& trg = targets[i];
            
            if (s.find(src, pos) != pos)
            {
                continue;
            }
            
            for (int j = 1; j < src.length(); ++j)
            {
                drops.insert(pos + j);
            }
            
            indjects[pos] = trg;
            
            ballance += trg.length() - src.length();
        }
        
        auto outs = std::string();
        outs.reserve(s.length() + ballance);
        for (int i = 0; i < s.length(); ++i)
        {
            if (drops.find(i) != drops.end())
            {
                continue;
            }
            auto indjitr = indjects.find(i);
            if (indjitr != indjects.end())
            {
                outs += indjitr->second;
                continue;
            }
            
            outs += s[i];
        }
        
        return outs;
    }
};
