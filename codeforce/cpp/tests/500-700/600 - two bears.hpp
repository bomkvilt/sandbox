#ifndef CODEFORCE_CPP_600_TWOBEARS_HPP
#define CODEFORCE_CPP_600_TWOBEARS_HPP

#include <cmath>


class TwoBears
{
public:
	static int GetTime(int a, int b)
	{
		// a 3^t > b 2^t
		// 1.5^t > b/a
		// t ln(1.5) > ln(b/a)
		// t > ln(b/a) / ln(1.5)
		return int(std::log((double)b / a) / std::log(1.5)) + 1;
	}
};


#if 0
#include <iostream>
int main()
{
	int a = 0;
	std::cin >> a;

	int b = 0;
	std::cin >> b;

	std::cout << TwoBears::GetTime(a, b) << std::endl;
}
#endif //!CODEFORCE_CPP_600_TWOBEARS_HPP
#endif //!CODEFORCE_CPP_600_TWOBEARS_HPP
