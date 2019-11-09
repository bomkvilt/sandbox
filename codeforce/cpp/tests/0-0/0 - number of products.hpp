#ifndef CODEFORCE_CPP_0_NUMBEROFPRODUCTS_HPP
#define CODEFORCE_CPP_0_NUMBEROFPRODUCTS_HPP

#include <tuple>
#include <vector>


// https://codeforces.com/problemset/problem/1215/B
class NumOfProds
{
public:
	static std::tuple<int64_t, int64_t> GetCounts(const std::vector<int>& vv)
	{
		int64_t n = 0;
		int64_t p = 0;
		int64_t nlen = 0;
		int64_t plen = 0;

		for (auto r = 0; r < vv.size(); ++r)
		{
			if (vv[r] < 0)
			{
				std::swap(nlen, plen);
				++nlen;
			}
			else ++plen;

			n += nlen;
			p += plen;
		}
		return { n, p };
	}
};


#if 0
#include <iostream>
int main()
{
	int n = 0;
	std::cin >> n;

	auto vv = std::vector<int>(n);
	vv.resize(n);
	for (int i = 0; i < n; i++)
	{
		int tmp = 0;
		std::cin >> tmp;
		vv[i] = tmp;
	}

	auto [neg, pos] = NumOfProds::GetCounts(vv);
	std::cout << neg << " " << pos << "\n";
}
#endif //!CODEFORCE_CPP_0_NUMBEROFPRODUCTS_HPP
#endif //!CODEFORCE_CPP_0_NUMBEROFPRODUCTS_HPP
