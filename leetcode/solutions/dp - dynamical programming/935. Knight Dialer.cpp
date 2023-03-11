// https://leetcode.com/problems/knight-dialer/description/


class Solution {
public:
    int knightDialer(int n) {
        static constexpr int MODULO = 1000 * 1000 * 1000  + 7;

        static const auto allowedMoves = std::vector<std::vector<int>>{
            /*0:*/ {4, 6},
            /*1:*/ {6, 8},
            /*2:*/ {7, 9},
            /*3:*/ {4, 8},
            /*4:*/ {0, 3, 9},
            /*5:*/ {},
            /*6:*/ {0, 1, 7},
            /*7:*/ {2, 6},
            /*8:*/ {1, 3},
            /*9:*/ {2, 4},
        };
        
        if (n < 1) {
            return 0;
        }
        
        using TCachemap = std::array<int, 10>;
        TCachemap cachemap;
        cachemap.fill(1);
        
        for (int i = 2; i <= n; ++i) {
            auto newmap = TCachemap{};
            newmap.fill(0);
            
            for (int sidx = 0; sidx < 10; ++sidx)
            for (int tidx : allowedMoves[sidx]) {
                newmap[tidx] = (newmap[tidx] + cachemap[sidx]) % MODULO;
            }
            
            cachemap = newmap;
        }
        
        return std::accumulate(
            cachemap.begin(), cachemap.end(), 0,
            [](const int a, const int b) {
                return (a + b) % MODULO;
            }                 
        );
    }
};
