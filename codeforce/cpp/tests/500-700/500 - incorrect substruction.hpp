#ifndef CODEFORCE_CPP_500_INCORRECTSUBSTRUCTION_HPP
#define CODEFORCE_CPP_500_INCORRECTSUBSTRUCTION_HPP



class IncorrectSubstruction
{
public:
	static int Substruct(int n, int k)
	{
		while (true)
		{
			auto end = n % 10;

			if (k && !end)
			{
				--k;
				n /= 10;
				continue;
			}

			if (k <= end)
			{
				return n - k;
			}
			k -= end;
			n -= end;
		}
	}
};


#if 0
#include <iostream>
int main()
{
	int n = 0;
	std::cin >> n;

	int k = 0;
	std::cin >> k;

	std::cout << IncorrectSubstruction::Substruct(n, k) << std::endl;
}
#endif //!CODEFORCE_CPP_500_INCORRECTSUBSTRUCTION_HPP
#endif //!CODEFORCE_CPP_500_INCORRECTSUBSTRUCTION_HPP
