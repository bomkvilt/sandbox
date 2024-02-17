// https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/

#include <array>
#include <cmath>
#include <assert.h>
#include <string>
#include <vector>


class Solution {
public:
    /**
     * NOTE: the algorithm is not optimal.
     * The problem can also be solved using a recursive DFS traversal that's 3 times faster.
     */
    std::vector<std::string> letterCombinations(const std::string& digits) {
        static constexpr auto MAPPING = std::array<std::string_view, 9>{
            "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz",
        };
        static constexpr size_t MAX_SIZE = 4;

        auto results = std::vector<std::string>{};
        results.reserve(std::pow(MAX_SIZE, digits.size()));

        const auto update = [&](const char d) -> void {
            const size_t digit = d - '2';
            assert(digit < MAPPING.size());

            const auto letters = MAPPING[digit];

            if (results.empty()) {
                for (const auto c : letters) {
                    std::string s {c};
                    s.reserve(digits.size());
                    results.push_back(std::move(s));
                }
                return;
            }

            const size_t resultSize = results.size();
            for (size_t idx = 0; idx < resultSize; ++idx) {
                for (auto letter = letters.begin(); letter < letters.end(); ++letter) {
                    if (letter == letters.begin()) {
                        results[idx] += *letter;
                        continue;
                    }

                    results.push_back(results[idx]);
                    results.back().back() = *letter;
                }
            }
        };

        for (const auto d : digits) {
            update(d);
        }

        return results;
    }
};
