// https://leetcode.com/problems/reconstruct-itinerary/description/

#include <algorithm>
#include <unordered_map>
#include <vector>


class Solution {
private:
    struct TState {
        std::vector<std::string> Iternaty;
        std::unordered_map<std::string, size_t> Quantity;
    };

public:
    std::vector<std::string> findItinerary(const std::vector<std::vector<std::string>>& tickets) {
        for (auto& ticket : tickets) {
            const auto& a = ticket[0];
            const auto& b = ticket[1];
            if (Quantity_[MakeKey(a, b)]++ == 0) {
                Adj_[a].push_back(b);
            }
            EdgesCount_ += 1;
        }

        for (auto& [a, bb] : Adj_) {
            std::ranges::sort(bb);
        }

        TState state;
        DFS("JFK", state);
        return state.Iternaty;
    }

private:
    static auto MakeKey(const std::string& a, const std::string& b) -> std::string {
        return a + ":" + b;
    };

    bool DFS(const std::string& a, TState& state) const {
        state.Iternaty.push_back(a);

        if (auto pos = Adj_.find(a); pos != Adj_.end()) {
            for (const auto& b : pos->second) {
                const auto key = MakeKey(a, b);
                const auto limit = Quantity_.at(key);
                auto& real = state.Quantity[key];

                if (limit == real) {
                    continue;
                }

                real += 1;

                if (DFS(b, state)) {
                    return true;
                }

                real -= 1;
            }
        }

        if (state.Iternaty.size() == EdgesCount_ + 1) {
            return true;
        }

        state.Iternaty.pop_back();
        return false;
    };

private:
    size_t EdgesCount_ = 0;
    std::unordered_map<std::string, size_t> Quantity_;
    std::unordered_map<std::string, std::vector<std::string>> Adj_;
};
