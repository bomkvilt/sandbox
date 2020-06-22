#ifndef CODEFORCE_CPP__800_134A_TEAM_HPP
#define CODEFORCE_CPP__800_134A_TEAM_HPP
#define PROBLEM_NAME Team
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	auto n = 0;
	std::cin >> n;

	auto a = 0;
	auto b = 0;
	auto c = 0;
	auto m = 0;
	while (n--)
	{
		std::cin >> a >> b >> c;
		if (a + b + c >= 2)
		{
			++m;
		}
	}

	std::cout << m << std::endl;
	return 0;
	return 0;
}


#endif //!CODEFORCE_CPP__800_134A_TEAM_HPP
