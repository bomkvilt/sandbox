// https://leetcode.com/problems/defanging-an-ip-address/description/


class Solution {
public:
    string defangIPaddr(string address) {
        constexpr static size_t OFFSET = 3 * 2;
        address.resize(address.size() + OFFSET);

        auto write_pos = address.rbegin();
        auto read_pos = address.rbegin() + OFFSET;
        auto end = address.rend();

        for (; read_pos != end && write_pos != end; ++read_pos) {
            if (*read_pos != '.') {
                *(write_pos++) = *read_pos;
                continue;
            }
            *(write_pos++) = ']';
            *(write_pos++) = '.';
            *(write_pos++) = '[';
        }

        assert(write_pos == end);
        return std::move(address);
    }
};
