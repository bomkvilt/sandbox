#ifndef CODEFORCE_CPP__800_APRIL2019A_TANOSSORT_HPP
#define CODEFORCE_CPP__800_APRIL2019A_TANOSSORT_HPP
#define PROBLEM_NAME TanosSort
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <vector>
#include <algorithm>



class PROBLEM_NAME
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

	std::cout << PROBLEM_NAME::GetMaxLen(vv) << "\n";
	return 0;
}


#endif //!CODEFORCE_CPP__800_APRIL2019A_TANOSSORT_HPP
