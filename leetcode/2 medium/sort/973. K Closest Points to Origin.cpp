class Solution 
{
public:
    
    using Point = vector<int>;
    
    vector<Point> kClosest(const vector<Point>& points, int k) 
    {
        auto norms = std::vector<std::pair<int, float>>();
        norms.reserve(points.size());
        
        for (int i = 0; i < points.size(); ++i)
        {
            norms.push_back({i, Norm(points[i])});
        }
        
        std::sort(norms.begin(), norms.end(), [this](const auto& l, const auto& r)
        {
            return l.second < r.second;
        });
        
        auto ans = vector<Point>();
        ans.reserve(k);
        
        for (int i = 0; i < k && i < norms.size(); ++i)
        {
            ans.push_back(points[norms[i].first]);
        }
        return ans;
    }
    
    static float Norm(const Point& point)
    {
        return std::sqrt(point[0] * point[0] + point[1] * point[1]);
    }
};
