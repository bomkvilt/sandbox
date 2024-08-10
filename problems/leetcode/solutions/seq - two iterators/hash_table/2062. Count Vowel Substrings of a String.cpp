// https://leetcode.com/problems/count-vowel-substrings-of-a-string

#include <algorithm>
#include <string_view>
#include <unordered_map>


class Solution {
public:
    int countVowelSubstrings(const std::string_view word) {
        static constexpr auto numVowels = 5;

        static constexpr auto isVowel = [](const char c) -> bool {
            switch (c) {
                case 'a': case 'e': case 'i': case 'o': case 'u':
                    return true;
                default:
                    return false;
            }
        };

        std::unordered_map<char, int> window;
        int prefix = 0;

        auto moveWindow = [&](auto& i, auto& j, auto s) -> void {
            auto nextit = std::find_if(j, s, isVowel);
            if (nextit != j) {
                window.clear();
                prefix = 0;
                i = j = nextit;
            }
            else {
                j = nextit;
            }
        };

        int result = 0;

        auto i = word.begin();
        auto j = word.begin();
        moveWindow(i, j, word.end());

        for (; j != word.end(); moveWindow(i, ++j, word.end())) {
            window[*j] += 1;

            // keep the minimum core
            for (; window[*i] > 1; ++i) {
                window[*i] -= 1;
                prefix += 1;
            }

            if (window.size() == numVowels) {
                result += prefix;
                result += 1;
            }
        }

        return result;
    }
};
