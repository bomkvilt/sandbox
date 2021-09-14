#include <algorithm>



class Solution 
{
public:
    auto merge(vector<vector<int>>& intervals) -> vector<vector<int>>
    {
        using Interval = vector<int>;
        
        if (intervals.size() < 2)
        {
            return intervals;
        }
        
        constexpr auto lessByFirst = [](const Interval& a, const Interval& b) -> bool
        {
            return a[0] < b[0];
        };
        std::sort(intervals.begin(), intervals.end(), lessByFirst);
        
        auto itr = intervals.begin();
        auto end = intervals.end();
        
        auto outIntervals = vector<Interval>();
        outIntervals.reserve(intervals.size());
        outIntervals.push_back(*itr++);
        
        for (; itr != end; ++itr)
        {
            auto& a = outIntervals.back();
            const auto& b = *itr;
            
            if (a[1] >= b[0])
            {
                a[1] = std::max(a[1], b[1]);
                continue;
            }
            
            outIntervals.push_back(b);
        }
        
        return outIntervals;
    }
};
