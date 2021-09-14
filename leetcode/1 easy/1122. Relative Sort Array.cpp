#include <unordered_map>



class Solution 
{
public:
    vector<int> relativeSortArray(const vector<int>& arr1, const vector<int>& arr2) 
    {
        auto charStat = std::unordered_map<int, int>();
        for (auto c : arr1)
        {
            ++charStat[c];
        }
        
        auto arro = std::vector<int>(arr1.size(), 0);
        auto itr = arro.begin();
        
        for (auto c : arr2)
        {
            const auto num = charStat[c];
            charStat.erase(c);
            
            for (int i = 0; i < num; ++i)
            {
                *(itr++) = c;
            }
        }
        
        auto begin = itr;
        
        for (const auto& [c, num] : charStat)
        {
            for (int i = 0; i < num; ++i)
            {
                *(itr++) = c;
            }
        }
        
        std::sort(begin, arro.end());
        
        return arro;
    }
};
