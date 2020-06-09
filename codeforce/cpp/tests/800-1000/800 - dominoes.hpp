#ifndef CODEFORCE_CPP__800_DOMINOES_HPP
#define CODEFORCE_CPP__800_DOMINOES_HPP
#define TESTNAME __

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>



TMAIN()
{
	auto N = 0;
	auto M = 0;
	std::cin >> N >> M;
	
	std::cout << int(N * M / 2) << std::endl;
	return 0;
}

#endif //!CODEFORCE_CPP__800_DOMINOES_HPP
