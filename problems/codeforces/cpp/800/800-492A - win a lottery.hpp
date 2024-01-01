#ifndef CODEFORCE_CPP__800_492A_WINALOTTERY_HPP
#define CODEFORCE_CPP__800_492A_WINALOTTERY_HPP
#define PROBLEM_NAME WinALottery
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	int m = 0;
	std::cin >> m;

	int n = 0;
	for (auto c : { 100, 20, 10, 5, 1 })
	{
		n += m / c;
		m %= c;
	}

	std::cout << n;
	return 0;
}


#endif //!CODEFORCE_CPP__800_492A_WINALOTTERY_HPP
