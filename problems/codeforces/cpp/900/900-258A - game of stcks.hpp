#ifndef CODEFORCE_CPP__900_258A_GAMEOFSTCKS_HPP
#define CODEFORCE_CPP__900_258A_GAMEOFSTCKS_HPP
#define PROBLEM_NAME GameOfStcks
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_258A_GAMEOFSTCKS_HPP
