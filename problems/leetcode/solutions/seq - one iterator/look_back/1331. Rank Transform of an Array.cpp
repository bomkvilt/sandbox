// https://leetcode.com/problems/rank-transform-of-an-array/description/

#include <algorithm>
#include <ranges>
#include <vector>


class Solution {
public:
    std::vector<int> arrayRankTransform(const std::vector<int>& arr) {
        struct TKey final {
            int value = 0;
            int idx = 0;
            int rank = 0;
        };

        if (arr.empty()) {
            return {};
        }

        std::vector<TKey> index;
        index.reserve(arr.size());

        for (const auto [idx, value] : std::views::enumerate(arr)) {
            index.push_back({.value = value, .idx = static_cast<int>(idx), .rank = 1});
        }

        std::ranges::sort(index, {}, &TKey::value);

        const auto& first = index.front();
        std::vector<int> ranks (arr.size(), {});
        ranks[first.idx] = first.rank;

        for (auto i = index.begin(), j = i + 1; j != index.end(); i = j, ++j) {
            ranks[j->idx] = j->rank = i->rank + (i->value != j->value);
        }
        return ranks;

    }
};
