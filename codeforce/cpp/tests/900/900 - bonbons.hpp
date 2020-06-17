#ifndef CODEFORCE_CPP__900_BONBONS_HPP
#define CODEFORCE_CPP__900_BONBONS_HPP
#define TESTNAME __

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>



TMAIN()
{
	auto t = 0;
	std::cin >> t;

	while (t--)
	{
		double n = 0;
		std::cin >> n;

		for (auto s = 4; true; s *= 2)
		{
			auto p = s - 1;
			auto x = n / p;
			if (int64_t(x) == x)
			{
				std::cout << int64_t(x) << std::endl;
				break;
			}
		}
	}
	return 0;
}

#endif //!CODEFORCE_CPP__900_BONBONS_HPP
