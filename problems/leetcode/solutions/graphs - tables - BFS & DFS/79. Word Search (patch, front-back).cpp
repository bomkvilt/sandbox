// https://leetcode.com/problems/word-search/description/

#include <ranges>
#include <string_view>
#include <vector>


class Solution {
public:
    using TBoard = std::vector<std::vector<char>>;

private:
    static constexpr char FILLER = '\0';

    struct TBacktracker {
        TBoard& Board;
        size_t X = 0;
        size_t Y = 0;

        TBacktracker(TBoard& board)
            : Board{board}
            , X{board.size()}
            , Y{board[0].size()}
        {}

        template <typename TI, typename TS>
        bool operator()(std::ranges::subrange<TI, TS> s) {
            for (int x = 0; x < X; ++x) {
                for (int y = 0; y < Y; ++y) {
                    if ((*this)(s, x, y)) {
                        return true;
                    }
                }
            }
            return false;
        }

        template <typename TI, typename TS>
        bool operator()(std::ranges::subrange<TI, TS> s, const int x0, const int y0) {
            if (x0 < 0 || x0 >= X || y0 < 0 || y0 >= Y) {
                return false;
            }

            const auto c = s.front();
            auto& ceil = Board[x0][y0];

            if (ceil != c) {
                return false;
            }

            if (s = s.next(); s.empty()) {
                return true;
            }

            ceil = FILLER;

            auto matched = false;
            if (!matched) { matched = (*this)(s, x0 + 0, y0 + 1); }
            if (!matched) { matched = (*this)(s, x0 + 1, y0 + 0); }
            if (!matched) { matched = (*this)(s, x0 + 0, y0 - 1); }
            if (!matched) { matched = (*this)(s, x0 - 1, y0 + 0); }

            ceil = c;
            return matched;
        }
    };

public:
    bool exist(TBoard& board, std::string_view word) {
        if (board.empty() || board[0].empty() || word.empty()) {
            return false;
        }

        TBacktracker tracker {board};

        int ballance = 0;
        for (int x = 0; x < tracker.X; ++x) {
            for (int y = 0; y < tracker.Y; ++y) {
                const auto c = board[x][y];
                ballance += c == word.front();
                ballance -= c == word.back();
            }
        }

        if (ballance > 0) {
            return tracker(std::ranges::subrange{word.rbegin(), word.rend()});
        }
        else {
            return tracker(std::ranges::subrange{word});
        }
    }
};


bool exist(Solution::TBoard& board, std::string_view word) {
    return Solution{}.exist(board, word);
}
