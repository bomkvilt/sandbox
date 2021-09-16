#include <unordered_map>
#include <unordered_set>
#include <stack>


/** Note: the solution is made to study a Tarjan's strongly connected algorithm
 *  and study a way to build DFS iteration process with no recursion.
 *  
 *  To make the sudying more clear the code is written with use of independed
 *  function and callbacks. This style leads to the fact that the code is slower
 *  than 95% of accepted attempts
 */
class Solution 
{    
public:
    
    using Conn  = std::vector<int>;
    using Graph = std::unordered_map<int, std::unordered_set<int>>;
    using Nodes = std::unordered_set<int>;
    
public:
    
    vector<Conn> criticalConnections(const int n, const vector<Conn>& connections) 
    {
        auto graph = Graph();
        auto nodes = Nodes();
        for (const auto& conn : connections)
        {
            graph[conn[0]].insert(conn[1]);
            graph[conn[1]].insert(conn[0]);
            nodes.insert(conn[0]);
            nodes.insert(conn[1]);
        }

        auto ans = std::vector<Conn>();
        TarjanClassterSwitchs(graph, ans);
        return ans;
    }

private:

    void TarjanClassterSwitchs(const Graph& graph, Nodes leftNodes, std::vector<Conn>& ans)
    {
        auto parents = std::unordered_map<int, int>();
        auto lowLink = std::unordered_map<int, int>();
        auto nodeIDs = std::unordered_map<int, int>();
        auto IDCounter = 0;

        while (leftNodes.size())
        {
            const auto U0 = leftNodes.top();
            leftNodes.pop();

            DFS3(U0, graph, leftNodes, parents
                , [&](const int u)
                {
                    nodeIDs[u] = IDCounter;
                    minLink[u] = IDCounter;
                    ++IDCounter;
                }
                , [&](const int u, const int p)
                {
                    lowLink[p] = std::min(lowLink[p], lowLink[u]);

                    if (lowLink[u] > nodeIDs[p])
                    {
                        ans.push_back({p, u});
                    }
                }
                , [&](const int u, const int v)
                {
                    lowLink[u] = std::min(lowLink[u], lowLink[v]);
                }
            );
        }
    }

    void DFS3(
          int U0
        , const Graph& graph
        , std::unordered_set<int>& leftNodes
        , std::unordered_map<int, int>& parents
        , std::function<void(int)> onVertexEnter
        , std::function<void(int, int)> onVertexExit
        , std::function<void(int, int)> onVisitetVertexHit
    ) {
        auto vertexStack = std::stack<int>();
        vertexStack.push(U0);

        while (vertexStack.size())
        {
            const auto u = vertexStack.top();

            // all child nodes are visited
            if (leftNodes.find(u) == leftNodes.end())
            {
                vertexStack.pop();

                auto paritr = parents.find(u);
                if (paritr != parents.end())
                {
                    onVertexExit(u, *paritr);
                }
                else
                {
                    onVertexExit(u, u);
                }
                continue;
            }

            // a new vertex is visited
            leftNodes.erase(u);
            onVertexEnter(u);

            for (const auto v : graph[u])
            {
                auto discitr = leftNodes.find(v);
                if (discitr != leftNodes.end())
                {
                    parents[v] = u;
                    vertexStack.push(v);
                    continue;
                }

                auto paritr = parents.find(u);
                if (paritr != parents.end())
                {
                    onVisitetVertexHit(u, v);
                    continue;
                }
            }
        }
    }
};
