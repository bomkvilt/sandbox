#ifndef CODEFORCE_CPP__900_FOOTBALL_HPP
#define CODEFORCE_CPP__900_FOOTBALL_HPP
#define TESTNAME FOOTBALL

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>



TMAIN()
{
	auto map = std::string();
	std::cin >> map;

	auto c = '1';
	auto l = 0;
	for (auto p : map)
	{
		if (p == c)
		{
			if (++l >= 7)
			{
				std::cout << "YES" << std::endl;
				return 0;
			}
		}
		else
		{
			l = 1;
			c = p;
		}
	}
	std::cout << "NO" << std::endl;
	return 0;
}

#endif //!CODEFORCE_CPP__900_FOOTBALL_HPP
