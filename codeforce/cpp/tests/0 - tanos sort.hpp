#ifndef CODEFORCE_CPP_0_TANOSSORT_HPP
#define CODEFORCE_CPP_0_TANOSSORT_HPP

#include <vector>
#include <algorithm>


// https://codeforces.com/problemset/problem/1145/A
class TanosSort
{
public:
	static int GetMaxLen(const std::vector<int>& vv)
	{
		return GetMaxLen(vv.begin(), vv.end());
	}

private:
	template<typename It>
	static int GetMaxLen(It begin, It end)
	{
		if (std::is_sorted(begin, end))
		{
			return std::distance(begin, end);
		}

		auto mid = begin + std::distance(begin, end) / 2;
		return std::max(
			GetMaxLen(begin, mid),
			GetMaxLen(mid, end)
		);
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

	std::cout << TanosSort::GetMaxLen(vv) << "\n";
}
#endif //!CODEFORCE_CPP_0_YELOWCARDS_HPP

#endif //!CODEFORCE_CPP_0_TANOSSORT_HPP
