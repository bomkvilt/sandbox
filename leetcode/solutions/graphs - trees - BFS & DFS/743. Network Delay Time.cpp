class Solution 
{
public:
    
    int networkDelayTime(const vector<vector<int>>& times, int n, int k) 
    {
        auto nodes = std::vector<int>(n + 1, INT_MAX);
        auto edges = std::vector<std::unordered_map<int, int>>(n + 1);
        
        for (auto& timeInfo : times)
        {
            const auto& src  = timeInfo[0];
            const auto& dst  = timeInfo[1];
            const auto& time = timeInfo[2];
            edges[src][dst] = time;
        }
        
        return Solve(nodes, edges, k);
    }
    
private:
    
    int Solve(std::vector<int>& nodeTimes, const std::vector<std::unordered_map<int, int>>& edges, const int k0)
    {
        // {signal time, dest_node}
        auto signals = std::multimap<int, int>();
        signals.insert({0, k0});
        
        while(signals.size())
        {
            const auto itr  = signals.begin();
            const auto time = itr->first;
            const auto targ = itr->second;
            signals.erase(itr);
            
            if (nodeTimes[targ] <= time)
            {
                continue;
            }
            nodeTimes[targ] = time;
            
            for (auto& [dest, dt] : edges[targ])
            {
                const auto t1 = time + dt;
                signals.insert({t1, dest});
            }
        }

        int maxTime = INT_MIN;
        for (int i = 1; i < nodeTimes.size(); ++i)
        {
            const auto time = nodeTimes[i];
            maxTime = std::max(maxTime, time);
        }
        return maxTime == INT_MAX ? -1 : maxTime;
    }
};
