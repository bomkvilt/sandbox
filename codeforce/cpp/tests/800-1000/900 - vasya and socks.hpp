#ifndef CODEFORCE_CPP__900_VASYAANDSOCKS_HPP
#define CODEFORCE_CPP__900_VASYAANDSOCKS_HPP
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
	auto m = 0;
	std::cin >> n >> m;

	for (int i = 1; true; ++i)
	{
		if (i % m == 0)
		{
			++n;
		}
		if (--n <= 0)
		{
			std::cout << i << std::endl;
			return 0;
		}
	}
	return 0;
}

#endif //!CODEFORCE_CPP__900_VASYAANDSOCKS_HPP
