#ifndef CODEFORCE_CPP__900_077A_FOOTBALL_HPP
#define CODEFORCE_CPP__900_077A_FOOTBALL_HPP
#define PROBLEM_NAME Football
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	auto map = std::string();
	std::cin >> map;

	auto c = '1';
	auto l = 0;
	for (auto p : map)
	{
		if (p == c)
		{
			if (++l >= 7)
			{
				std::cout << "YES" << std::endl;
				return 0;
			}
		}
		else
		{
			l = 1;
			c = p;
		}
	}
	std::cout << "NO" << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__900_077A_FOOTBALL_HPP
