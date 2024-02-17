// https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/

#include <array>
#include <cmath>
#include <string>
#include <vector>


// https://godbolt.org/z/9johMhsv3
// NOTE: cannot be compiled with clang++ for now
// https://libcxx.llvm.org/Status/Cxx23.html#cxx23-status function/packaged_task deduction guides and deducing this
class Solution {
public:
    std::vector<std::string> letterCombinations(const std::string& digits) {
        static constexpr auto MAPPING = std::array<std::string_view, 9>{
            "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz",
        };
        static constexpr size_t MAX_SIZE = 4;

        if (digits.empty()) {
            return {};
        }

        auto results = std::vector<std::string>{};
        results.reserve(std::pow(MAX_SIZE, digits.size()));

        auto update = [&](this auto& self, std::string_view rest, std::string& state) -> void {
            if (rest.empty()) {
                results.push_back(state);
                return;
            }

            const auto d = rest.front();
            rest.remove_prefix(1);

            const auto idx = d - '2';  // NOTE: 2..9
            for (auto c : MAPPING[idx]) {
                state.push_back(c);
                self(rest, state);
                state.pop_back();
            }
        };

        std::string state;
        update(digits, state);
        return results;
    }
};
