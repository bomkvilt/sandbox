#include <array>
#include <queue>



class Solution 
{
    using WindowSize = std::array<int, 2>;
    using Point      = std::array<int, 2>;

    struct PointHash
    {
        size_t operator()(const WindowSize& size) const
        {   // http://stackoverflow.com/a/1646913/126995
            constexpr auto hash = std::hash<int>();

            size_t outhash = 17;
            outhash = outhash * 31 + hash(size[0]);
            outhash = outhash * 31 + hash(size[1]);
            return outhash;
        }
    };

public:

    int numSubmat(const vector<vector<int>>& mat) 
    {
        
    }

private:

    int Solve(const vector<vector<int>>& mat)
    {
        const auto M = mat   .size();
        const auto N = mat[0].size();

        struct CaseOrder
        {
            WindowSize windowSize;
            std::vector<Point> points;
            bool direction;
        };

        auto orders = std::queue<CaseOrder>();
        orders.emplace({{0, 0}, GetInitialPoints(mat), 0});

        while (orders.size())
        {
            const auto& order = orders.top();
            
            for (auto point : order.points)
            {
                const auto direction = !order.direction;
                const auto windowDim = order.windowSize[direction];
                for (int i = 0; i < windowDim; ++i, ++point[direction])
                {
                    if (mat[point[0]][point[1]] != 1)
                    {
                        
                    }
                }
            }
        }
    }

    std::vector<int> GetInitialPoints(const vector<vector<int>>& mat) const
    {
        const auto M = mat   .size();
        const auto N = mat[0].size();

        auto points = std::vector<int>();
        for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
        {
            if (mat[i][j] != 0)
            {
                points.emplace_back({i, j});
            }
        }
        return points;
    }
};
