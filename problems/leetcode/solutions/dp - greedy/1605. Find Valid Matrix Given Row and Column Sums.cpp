// https://leetcode.com/problems/find-valid-matrix-given-row-and-column-sums

#include <ranges>
#include <vector>


class Solution {
public:
    std::vector<std::vector<int>> restoreMatrix(
        std::vector<int> rowSums,
        std::vector<int> colSums
    ) {
        std::vector<std::vector<int>> result(rowSums.size());
        for (const auto i : std::views::iota(0ul, rowSums.size())) {
            result[i].resize(colSums.size(), 0);
        }

        for (const auto i : std::views::iota(0ul, rowSums.size())) {
            for (const auto j : std::views::iota(0ul, colSums.size())) {
                const auto used = std::min(rowSums[i], colSums[j]);
                rowSums[i] -= used;
                colSums[j] -= used;
                result[i][j] = used;
            }
        }
        return result;
    }
};
