#ifndef CODEFORCE_CPP__1400_585B_NUMBEROFPRODUCTS_HPP
#define CODEFORCE_CPP__1400_585B_NUMBEROFPRODUCTS_HPP
#define PROBLEM_NAME NumberOfProducts
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <tuple>
#include <vector>



auto PROBLEM_NAME(const std::vector<int>& vv) -> std::tuple<int64_t, int64_t>
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


PROBLEM_MAIN()
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

	auto [neg, pos] = PROBLEM_NAME(vv);
	std::cout << neg << " " << pos << "\n";
	return 0;
}


#endif //!CODEFORCE_CPP__1400_585B_NUMBEROFPRODUCTS_HPP
