#ifndef CODEFORCE_CPP__600_FAFA_HPP
#define CODEFORCE_CPP__600_FAFA_HPP


#if 0
#include <iostream>
int main()
{
	int c = 0;
	std::cin >> c;

	// c = n (m + 1)
	// m = c / n - 1
	int count = 0;
	for (int n = 1; n <= c / 2; ++n)
	{
		if (c % n == 0)
		{
			++count;
		}
	}
	std::cout << count << std::endl;
}
#endif //!CODEFORCE_CPP_600_FAFA_HPP
#endif //!CODEFORCE_CPP__600_FAFA_HPP
