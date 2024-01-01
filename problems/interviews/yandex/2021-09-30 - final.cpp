// (1 1 1 2 2)  <-->  (1, 3) (2, 2)
// (1 1 1 2 2)  <-->  (1, 3) (2, 2)


using SVector = std::vector<std::pair<int, int>>;

int dotProduct(const SVector& v1, const SVector& v2)
{
    int sum = 0;
    
    int r1 = 0;
    int r2 = 0;
    auto itr1 = v1.begin();
    auto itr2 = v2.being();
    auto end1 = v1.end();
    auto end2 = v2.end();
    
    for (; itr1 != end1 && itr2 != end2;)
    {
        const auto n = std::min(itr1->second - r1, itr2->second - r2);
        sum += itr1->first * itr2->first * n;
        
        r1 += n;
        r2 += n;
        if (r1 == itr1->second) { ++itr1; r1 = 0; }
        if (r2 == itr2->second) { ++itr2; r2 = 0; }
    }
    if (itr1 != end1 || itr2 != end2)
    {
        throw std::runtime_error("vectors have ...");
    }
    return sum;
}

// test 1
// (1, 3) (2, 2)
// (2, 5)
// 10

// test 2
// 
// (1, 3)
// exc

// test 3
// (1, 3)
// 
// exc

// test
// (1, 3) (2, 2)
// (2, 4)
// exc

