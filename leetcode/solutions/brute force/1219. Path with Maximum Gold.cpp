class Solution {
public:
    int getMaximumGold(vector<vector<int>>& grid) {
        const auto deltas = std::vector<std::pair<int, int>>({
            {-1, 0}, {+1, 0}, {0, +1}, {0, -1},
        });
        
        const auto n = grid.size();
        if (!n) {
            return 0;
        }

        const auto m = grid[0].size();
        if (!m) {
            return 0;
        }
        
        int result = 0;
        for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            result = std::max(result, DFS(grid, deltas, i, j));
        }
        return result;
    }

private:
    int DFS(vector<vector<int>>& grid, const std::vector<std::pair<int, int>>& deltas, int i, int j) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size() || !grid[i][j]) {
            return 0;
        }
        
        int result = 0;
        const auto x = grid[i][j];
        grid[i][j] = 0;
        
        for (const auto& [di, dj]: deltas) {
            result = std::max(result, x + DFS(grid, deltas, i + di, j + dj));
        }
        
        grid[i][j] = x;
        return result;
    }
};
