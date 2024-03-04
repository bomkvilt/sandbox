// https://leetcode.com/problems/word-search/description/

#include <ranges>
#include <string_view>
#include <vector>


class Solution {
public:
    using TBoard = std::vector<std::vector<char>>;

private:
    struct TBacktracker {
    public:
        const TBoard& Board;
        size_t X = 0;
        size_t Y = 0;

        std::vector<bool> Visited;

    public:
        TBacktracker(const TBoard& board)
            : Board{board}
            , X{board.size()}
            , Y{board[0].size()}
        {
            Visited.resize(X * Y, false);
        }

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
        bool operator()(std::ranges::subrange<TI, TS> s, const int x, const int y) {
            if (x < 0 || x >= X || y < 0 || y >= Y) {
                return false;
            }

            const size_t idx = x * Y + y;
            if (Board[x][y] != s.front() || Visited[idx]) {
                return false;
            }

            if (s = s.next(); s.empty()) {
                return true;
            }

            Visited[idx] = true;

            auto matched = false;
            if (!matched) { matched = (*this)(s, x + 0, y + 1); }
            if (!matched) { matched = (*this)(s, x + 1, y + 0); }
            if (!matched) { matched = (*this)(s, x + 0, y - 1); }
            if (!matched) { matched = (*this)(s, x - 1, y + 0); }

            Visited[idx] = false;
            return matched;
        }
    };

public:
    bool exist(const TBoard& board, std::string_view word) {
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
