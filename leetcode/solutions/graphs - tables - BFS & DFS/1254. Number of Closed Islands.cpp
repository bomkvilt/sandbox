class Solution {
public:
    int closedIsland(vector<vector<int>>& grid) {
        const int m = grid.size();
        if (!m) {
            return 0;
        }

        const int n = grid[0].size();
        if (!n) {
            return 0;
        }
        
        const auto steps = std::vector<std::pair<int, int>>({
            {-1, 0}, {+1, 0}, {0, +1}, {0, -1},
        });

        int islands = 0;
        for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] != 0) {
                continue;
            }
            if (ColorMap(grid, steps, i, j)) {
                ++islands;
            }
        }
        return islands;
    }
    
private:
    bool ColorMap(vector<vector<int>>& grid, const std::vector<std::pair<int, int>>& steps, int i0, int j0) {
        const int im = grid.size();
        if (!im) {
            return false;
        }

        const int jm = grid[0].size();
        if (!jm) {
            return false;
        }

        std::vector<std::pair<int, int>> dfs;
        dfs.push_back({i0, j0});

        bool isIsland = true;
        while (dfs.size()) {
            const auto [is, js] = dfs.back();
            dfs.pop_back();

            for (const auto& [di, dj] : steps) {
                auto ix = is + di;
                auto jx = js + dj;
                if (ix < 0 || ix >= im || jx < 0 || jx >= jm) {
                    isIsland = false;
                    continue;
                }
                if (grid[ix][jx] == 0) {
                    grid[ix][jx] = -1;
                    dfs.push_back({ix, jx});
                }
            }
        }
        return isIsland;
    }
};
