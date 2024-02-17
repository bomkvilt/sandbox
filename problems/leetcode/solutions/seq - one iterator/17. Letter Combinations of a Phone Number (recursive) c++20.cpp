// https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/

#include <array>
#include <cmath>
#include <string>
#include <vector>


// https://godbolt.org/z/YEbGz6baP
class Solution {
private:
    static constexpr auto MAPPING = std::array<std::string_view, 9>{
        "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz",
    };
    static constexpr size_t MAX_SIZE = 4;

public:
    std::vector<std::string> letterCombinations(const std::string& digits) {
        if (digits.empty()) {
            return {};
        }

        auto results = std::vector<std::string>{};
        results.reserve(std::pow(MAX_SIZE, digits.size()));

        std::string state;
        Update(results, digits, state);
        return results;
    }

private:
    // NOTE: can be simplified using a deduced this
    void Update(std::vector<std::string>& results, std::string_view rest, std::string& state) {
        if (rest.empty()) {
            results.push_back(state);
            return;
        }

        const auto d = rest.front();
        rest.remove_prefix(1);

        const auto idx = d - '2';  // NOTE: 2..9
        for (auto c : MAPPING[idx]) {
            state.push_back(c);
            Update(results, rest, state);
            state.pop_back();
        }
    }
};
