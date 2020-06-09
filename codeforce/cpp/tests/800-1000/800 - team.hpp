#ifndef CODEFORCE_CPP__800_TEAM_HPP
#define CODEFORCE_CPP__800_TEAM_HPP
#define TESTNAME __

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>



TMAIN()
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
}

#endif //!CODEFORCE_CPP__800_TEAM_HPP
