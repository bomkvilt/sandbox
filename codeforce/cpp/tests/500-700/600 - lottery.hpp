#ifndef CODEFORCE_CPP__600_LOTTERY_HPP
#define CODEFORCE_CPP__600_LOTTERY_HPP


#if 0
#include <iostream>
int main()
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
}
#endif //!CODEFORCE_CPP_600_LOTTERY_HPP
#endif //!CODEFORCE_CPP__600_LOTTERY_HPP
