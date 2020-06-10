#ifndef CODEFORCE_CPP__900_TWINS_HPP
#define CODEFORCE_CPP__900_TWINS_HPP
#define TESTNAME __

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <assert.h>
#include <iostream>
#include <cmath>
#include <map>



TMAIN()
{
	struct Counter
	{
		int value = 0;

		int operator=(int rhs)
		{
			value = rhs;
		}

		operator int() const
		{
			return value;
		}

		int operator++()
		{
			return ++value;
		}

		int operator+=(int rhs)
		{
			return value += rhs;
		}
	};

	auto n = 0;
	std::cin >> n;

	auto summ = 0;
	auto coins = std::map<int, Counter, std::greater<int>>();
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


#endif //!CODEFORCE_CPP__900_TWINS_HPP
