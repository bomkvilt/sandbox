#ifndef CODEFORCE_CPP__900_188A_EVENANDODD_HPP
#define CODEFORCE_CPP__900_188A_EVENANDODD_HPP
#define PROBLEM_NAME EvenAndOdd
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_188A_EVENANDODD_HPP
