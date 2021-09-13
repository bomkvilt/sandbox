// Дан массив точек с целочисленными координатами (x, y). Определить, 
// существует ли вертикальная прямая, делящая все точки, не лежащие на ней, 
// на 2 симметричных относительно этой прямой множества.

// ._!_.
// _.!._ => true

// __!_.
// _.!._ => false


using Point = std::array<int, 2>;



bool foo(const std::vector<Point>& points)
{   
    if (points.size() == 0)
    {
        return true;
    }
    
    auto pointDB = std::unordered_set<Point>();
        
    int xmin = INT_MAX;
    int xmax = INT_MIN;
    for (auto& point : points)
    {
        xmin = std::min(xmin, point[0]);
        xmax = std::max(xmax, point[0]);
        pointDB.insert(point);
    }

    const auto cx2 = xmax - xmin;
    if (cx2 % 2)
    {
        return false;
    }

    for (const auto& point : pointDB)
    {
        const int  x1 = cx2 - point[0];
        const auto p1 = Point{x1, point[1]};

        if (pointDB.find(p1) == pointDB.end())
        {
            return false;
        }
    }
    return true;
}
