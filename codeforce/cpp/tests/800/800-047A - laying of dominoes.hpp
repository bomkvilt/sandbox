#ifndef CODEFORCE_CPP__800_047A_LAYINGOFDOMINOES_HPP
#define CODEFORCE_CPP__800_047A_LAYINGOFDOMINOES_HPP
#define PROBLEM_NAME LayingOfDominoes
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	auto N = 0;
	auto M = 0;
	std::cin >> N >> M;
	std::cout << int(N * M / 2) << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_047A_LAYINGOFDOMINOES_HPP
