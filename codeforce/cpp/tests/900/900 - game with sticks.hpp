#ifndef CODEFORCE_CPP__900_GAMEWITHSTICKS_HPP
#define CODEFORCE_CPP__900_GAMEWITHSTICKS_HPP
#define TESTNAME GAMEWITHSTICKS

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


	auto k = n < m ? n : m;
	std::cout << (k % 2
		? "Akshat"
		: "Malvika"
		) << std::endl;

	return 0;
}

#endif //!CODEFORCE_CPP__900_GAMEWITHSTICKS_HPP
