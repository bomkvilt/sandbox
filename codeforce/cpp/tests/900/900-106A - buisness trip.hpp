#ifndef CODEFORCE_CPP__900_106A_BUISNESSTRIP_HPP
#define CODEFORCE_CPP__900_106A_BUISNESSTRIP_HPP
#define PROBLEM_NAME BuisnessTrip
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <functional>
#include <map>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_106A_BUISNESSTRIP_HPP
