class Solution 
{
public:
    
    int minReorder(int n, const vector<vector<int>>& connections) 
    {
        auto edges = std::vector<std::set<int>>(n);
        auto roads = std::vector<std::set<int>>(n);
        for (auto& conn : connections)
        {
            roads[conn[0]].insert(conn[1]);
            edges[conn[0]].insert(conn[1]);
            edges[conn[1]].insert(conn[0]);
        }
        
        return BFS(0, edges, roads);
    }
    
    int BFS(int V0
        , const std::vector<std::set<int>>& edges
        , const std::vector<std::set<int>>& roads
    )
    {
        int missDirs = 0;
        auto visited = std::set<int>();
        
        auto queue = std::queue<int>();
        visited.insert(V0);
        queue.push(V0);
        
        while (queue.size())
        {
            const auto u = queue.front();
            queue.pop();
            
            // visit 'child' nodes
            for (auto& v : edges[u])
            {
                if (visited.find(v) != visited.end())
                {
                    continue;
                }
                visited.insert(v);
                queue.push(v);
                
                // check road direction
                auto& roadDB = roads[u];
                if (roadDB.find(v) != roadDB.end())
                {
                    ++missDirs;
                }
            }
        }
        return missDirs;
    }
};
