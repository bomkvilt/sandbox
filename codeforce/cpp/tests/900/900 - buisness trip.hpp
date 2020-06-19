#ifndef CODEFORCE_CPP__900_BUISNESSTRIP_HPP
#define CODEFORCE_CPP__900_BUISNESSTRIP_HPP
#define TESTNAME BUISNESSTRIP

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>
#include <functional>
#include <map>



TMAIN()
{
	auto n = 0;
	auto k = 12;
	std::cin >> n;

	auto list = std::map<int, int, std::greater<int>>();
	while (k--)
	{
		auto tmp = 0;
		std::cin >> tmp;
		list[tmp] += 1;
	}

	auto c = 0;
	for (auto [a, m] : list)
	{
		auto req = (n - c) / a;
		if (req < m)
		{
			c += req;
			break;
		}

		n -= a * m;
	}

	std::cout << c << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__900_BUISNESSTRIP_HPP
