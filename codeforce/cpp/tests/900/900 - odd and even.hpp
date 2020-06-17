#ifndef CODEFORCE_CPP__900_ODDANDEVEN_HPP
#define CODEFORCE_CPP__900_ODDANDEVEN_HPP
#define TESTNAME __

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>



TMAIN()
{
	int64_t n = 0;
	int64_t k = 0;
	std::cin >> n >> k;

	if (int64_t p = ceil(n / 2.); k <= p)
	{
		std::cout << k * 2 - 1 << std::endl;
	}
	else
	{
		std::cout << (k - p) * 2 << std::endl;
	}
	return 0;
}


#endif //!CODEFORCE_CPP__900_ODDANDEVEN_HPP
