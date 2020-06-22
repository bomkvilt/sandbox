#ifndef CODEFORCE_CPP__800_479A_INCORRECTSUBSTRUCTIONS_HPP
#define CODEFORCE_CPP__800_479A_INCORRECTSUBSTRUCTIONS_HPP
#define PROBLEM_NAME IncorrectSubstructions
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



int PROBLEM_NAME(int n, int k)
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


PROBLEM_MAIN()
{
	int n = 0;
	int k = 0;
	std::cin >> n >> k;
	std::cout << PROBLEM_NAME(n, k) << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_479A_INCORRECTSUBSTRUCTIONS_HPP
