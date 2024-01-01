// https://leetcode.com/problems/find-winner-on-a-tic-tac-toe-game/description/

class Solution {
public:
    string tictactoe(vector<vector<int>>& moves) {
        static constexpr auto idxRow  = [](int idx) { return 0 + idx; };
        static constexpr auto idxCol  = [](int idx) { return 3 + idx; };
        static constexpr auto idxDiag = [](int idx) { return 6 + idx; };
        
        std::vector<int> lineBallance(8, 0);
        int steps = 0;
        
        bool byA = true;
        for (const auto& move : moves) {
            const auto& x = move[0];
            const auto& y = move[1];

            const int diff = 1 - 2 * (!byA);
            const int winScore = 3;
            
            bool won = false;
            auto update = [&](int idx) {
                lineBallance[idx] += diff;
                auto state = lineBallance[idx];
                won |= state == winScore || state == -winScore;
            };

            update(idxCol(x));
            update(idxRow(y));

            if (x == y) {
                update(idxDiag(0));
            }
            if (y == 2 - x) {
                update(idxDiag(1));
            }
            
            if (won) {
                return byA ? "A" : "B";
            }

            byA = !byA;
            ++steps;
        }

        return steps == 3 * 3 ? "Draw" : "Pending";
    }
};
