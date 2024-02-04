// https://leetcode.com/problems/reorder-data-in-log-files/description/

#include <algorithm>
#include <cctype>
#include <string_view>
#include <vector>
#include <string>


class Solution {
private:
    static std::string_view ParseValue(const std::string_view row) {
        return row.substr(row.find(' ') + 1);
    }

public:
    /**
     * O(n log(n) len(id) len(val))
     * de-factor slower than the (infos) approach: 5ms vs. 3ms
     */
    std::vector<std::string> reorderLogFiles(std::vector<std::string>& logs) {
        std::ranges::stable_sort(logs, [](const std::string& l, const std::string& r) -> bool {
            // digits are kept in the same relative order
            if (std::isdigit(l.back())) {
                return false;
            }
            if (std::isdigit(r.back())) {
                return true;
            }

            // strings are sorted by {<value>, <identifier>}
            auto lvalue = ParseValue(l);
            auto rvalue = ParseValue(r);

            return lvalue < rvalue || ((lvalue == rvalue) && l < r);
        });
        return logs;
    }
};
