// https://leetcode.com/problems/reconstruct-itinerary/description/

#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>


class Solution {
private:
    struct TState final {
        std::unordered_map<std::string, std::multiset<std::string>> Adj;
        std::vector<std::string> Itinerary;
    };

public:
    std::vector<std::string> findItinerary(std::vector<std::vector<std::string>>& tickets) {
        TState state;
        for (const auto& ab : tickets) {
            state.Adj[ab[0]].insert(ab[1]);
        }

        DFS(state, "JFK");

        std::ranges::reverse(state.Itinerary);
        return state.Itinerary;
    }

private:
    void DFS(TState& state, std::string airport) {
        while (!state.Adj[airport].empty()) {
            auto& adj = state.Adj[airport];
            auto pos = adj.begin();

            auto next = std::move(*pos);
            adj.erase(pos);

            DFS(state, next);
        }
        state.Itinerary.push_back(airport);
    }
};
