#ifndef CODEFORCE_CPP__2600_GLOB8F_LIGHTBULBSINACYCLE_HPP
#define CODEFORCE_CPP__2600_GLOB8F_LIGHTBULBSINACYCLE_HPP
#define PROBLEM_NAME LightBulbsInACycle
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <vector>
#include <cmath>



PROBLEM_MAIN()
{
	auto n = 0;
	std::cin >> n;

	auto R = [n](int k)->int
	{
		return (n - k) * (k - 1) / k;
	};

	// x is maximal when k = sqrt(n)
	int k = std::sqrt(n);
	int X = R(k);
	if (int X1 = R(k + 1); X1 > X)
	{
		k = k + 1;
		X = X1;
	}
	
	auto count = 0;
	auto bulbs = std::vector<bool>(n, 0);
	while (count < X)
	{
		std::cout << k;
		for (int i = 0, j = 0; j < k; i = ++i)
		{
			if (i % k && !bulbs[i])
			{
				std::cout << " " << i + 1;
				bulbs[i] = true;
				++count;
				++j;
			}
		}
		std::cout << std::endl;

		int i = 0;
		std::cin >> i;
		for (int j = 0; j < k; ++j)
		{
			if (bulbs[(i + j - 1) % n])
			{
				bulbs[(i + j - 1) % n] = false;
				--count;
			}
		}
	}
	std::cout << 0 << std::endl;

	return 0;
}


#endif //!CODEFORCE_CPP__2600_GLOB8F_LIGHTBULBSINACYCLE_HPP
