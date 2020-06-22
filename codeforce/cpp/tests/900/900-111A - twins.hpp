#ifndef CODEFORCE_CPP__900_111A_TWINS_HPP
#define CODEFORCE_CPP__900_111A_TWINS_HPP
#define PROBLEM_NAME Twins
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <assert.h>
#include <cmath>
#include <map>



PROBLEM_MAIN()
{
	auto n = 0;
	std::cin >> n;

	auto summ = 0;
	auto coins = std::map<int, int, std::greater<int>>();
	while (n--)
	{
		int coin = 0;
		std::cin >> coin;
		coins[coin] += 1;
		summ += coin;
	}
	
	auto cash = 0;
	auto ncon = 0;
	auto goal = summ / 2 + 1;
	for (auto [coin, count] : coins)
	{
		auto miss = goal - cash;
		auto need = (int)std::ceil((float)miss / coin);
		auto have = need > count ? count : need;
		cash += coin * have;
		ncon += have;

		if (cash >= goal)
		{
			break;
		}
	}
	assert(cash >= goal);

	std::cout << ncon << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__900_111A_TWINS_HPP
