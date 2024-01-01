#ifndef CODEFORCE_CPP__900_636A_BONBONS_HPP
#define CODEFORCE_CPP__900_636A_BONBONS_HPP
#define PROBLEM_NAME Bonbons
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	auto t = 0;
	std::cin >> t;

	while (t--)
	{
		double n = 0;
		std::cin >> n;

		for (auto s = 4; true; s *= 2)
		{
			auto p = s - 1;
			auto x = n / p;
			if (int64_t(x) == x)
			{
				std::cout << int64_t(x) << std::endl;
				break;
			}
		}
	}
	return 0;
}


#endif //!CODEFORCE_CPP__900_636A_BONBONS_HPP
