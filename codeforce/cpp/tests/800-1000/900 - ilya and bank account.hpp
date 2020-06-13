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
	auto n = 0;
	std::cin >> n;

	if (n > 0)
	{
		std::cout << n << std::endl;
	}
	else
	{
		auto max = n;
		auto a = 1;
		auto b = 10;
		for (auto k = 0; k <= 1; ++k)
		{
			auto tmp = (n / b * a) + (n % a);
			max = max > tmp ? max : tmp;

			a = b;
			b *= 10;
		}
		std::cout << max << std::endl;
	}

	return 0;
}

#endif //!CODEFORCE_CPP__900_BONBONS_HPP
