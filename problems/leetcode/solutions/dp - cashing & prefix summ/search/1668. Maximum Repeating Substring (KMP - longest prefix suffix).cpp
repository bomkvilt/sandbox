// https://leetcode.com/problems/maximum-repeating-substring

#include <string>
#include <vector>


class Solution {
public:
    int maxRepeating(const std::string& sequence, const std::string& word) {
        const auto pattern = [&]() {
            std::string result;
            result.reserve(word.size() + 1 + sequence.size());
            result += word;
            result += '\0';
            result += sequence;
            return result;
        }();

        // longest prefix-suffix function p(size)
        std::vector<int> lps (pattern.size() + 1, 0);
        lps[0] = -1; // crutch: p("") = -1

        // NOTE: size <= idx - 1
        for (size_t idx = 0, size = -1; idx < pattern.size(); ) {
            for (; size != -1 && pattern[idx] != pattern[size]; size = lps[size]) {
                // empty
            }
            lps[++idx] = ++size;
        }

        int maxK = 0;

        for (int sizeI = 0; sizeI < lps.size(); ++sizeI) {
            if (lps[sizeI] != word.size()) {
                continue;
            }

            int k = 0;
            for (int sizeJ = sizeI; sizeJ < lps.size() && lps[sizeJ] == word.size(); ++k) {
                sizeI = sizeJ;
                sizeJ += word.size();
            }

            maxK = std::max(maxK, k);
        }

        return maxK;
    }
};
