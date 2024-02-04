// https://leetcode.com/problems/reorder-data-in-log-files/description/

#include <assert.h>
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>


class Solution {
private:
    struct TRowInfo final {
    public:
        size_t Idx = 0;
        bool IsString = false;
        std::string_view Identifier;
        std::string_view Value;
    };
    
    struct TRowInfoOrder {
        inline bool operator()(const TRowInfo& l, const TRowInfo& r) const noexcept {
            if (l.IsString != r.IsString) {
                return l.IsString;
            }
            if (l.IsString) {
                return (l.Value < r.Value) || (l.Value == r.Value && l.Identifier <= r.Identifier);
            }
            else {
                return l.Idx <= r.Idx;
            }
        }
    };

    static TRowInfo ParseRow(std::string_view row, size_t idx) {
        const size_t idEnd = row.find(' ');
        const size_t plEnd = row.size();
        const size_t plPos = std::min(idEnd + 1, plEnd);
        assert(plPos < plEnd);

        return TRowInfo {
            .Idx = idx,
            .IsString = !std::isdigit(row[plPos]),
            .Identifier = row.substr(0, idEnd),
            .Value = row.substr(plPos, plEnd),
        };
    }

public:
    /**
     * O(n len(id) + n log(n) len(val))
     */
    std::vector<std::string> reorderLogFiles(std::vector<std::string>& logs) {
        std::vector<TRowInfo> infos;
        infos.reserve(logs.size());

        for (const auto& row : logs) {
            infos.push_back(ParseRow(row, infos.size()));
        }

        std::ranges::sort(infos, TRowInfoOrder{});

        std::vector<std::string> result;
        result.reserve(logs.size());

        for (const auto& info : infos) {
            result.push_back(std::move(logs[info.Idx]));
        }

        return result;
    }
};
