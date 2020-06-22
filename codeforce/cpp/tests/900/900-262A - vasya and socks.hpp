#ifndef CODEFORCE_CPP__900_262A_VASYAANDSOCKS_HPP
#define CODEFORCE_CPP__900_262A_VASYAANDSOCKS_HPP
#define PROBLEM_NAME VasyaAndSocks
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_262A_VASYAANDSOCKS_HPP
