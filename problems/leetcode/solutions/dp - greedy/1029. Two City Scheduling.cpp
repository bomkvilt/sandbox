class Solution 
{
public:
    
    int twoCitySchedCost(vector<vector<int>> costs) 
    {
        std::sort(costs.begin(), costs.end(), [](const auto& C0, const auto& C1)
        {
            return (C0[1] - C0[0]) < (C1[1] - C1[0]);
        });
        
        const auto n = costs.size() / 2;
        
        int cost = 0;
        for (int i = 0, j = 2*n - 1; i < n; ++i, --j)
        {
            cost += costs[i][1];
            cost += costs[j][0];
        }
        return cost;
    }
};
