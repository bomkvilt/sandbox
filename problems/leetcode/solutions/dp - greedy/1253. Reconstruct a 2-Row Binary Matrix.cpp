// https://leetcode.com/problems/reconstruct-a-2-row-binary-matrix

#include <ranges>
#include <vector>

// r[i] = sum{j} a[i][j] -> i -> a + b < a * b -> multiple solutions
// c[j] = sum{i} s[i][j] -> j

class Solution {
public:
    std::vector<std::vector<int>> reconstructMatrix(
        int upper,
        int lower,
        const std::vector<int>& colsums
    ) {
        std::vector<std::vector<int>> result(2);
        for (const auto i : std::views::iota(0ul, 2ul)) {
            result[i].resize(colsums.size(), 0);
        }

        for (const auto [i, colsum] : std::views::enumerate(colsums)) {
            switch (colsum) {
                // NOTE: we can face colsum = 2
                // -> we need to ballance rows
                case 1: {
                    if (upper > lower) {
                        result[0][i] = 1;
                        --upper;
                    }
                    else {
                        result[1][i] = 1;
                        --lower;
                    }
                    break;
                }
                case 2: {
                    result[0][i] = 1; --upper;
                    result[1][i] = 1; --lower;
                    break;
                }
            }
        }

        if (upper || lower) {
            return {};
        }
        return result;
    }
};
