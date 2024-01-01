#ifndef CODEFORCE_CPP__800_CUP2012_1A_NEXTROUND_HPP
#define CODEFORCE_CPP__800_CUP2012_1A_NEXTROUND_HPP
#define PROBLEM_NAME NextRound
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	auto n = 0;
	auto k = 0;
	std::cin >> n >> k;

	auto v = 0;
	auto w = 0;
	auto m = 0;
	while (n--)
	{
		std::cin >> v;
		if (v <= 0)
		{
			continue;
		}
		if (m < k || w == v)
		{
			w = v;
			++m;
		}
		else break;
	}

	std::cout << m << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_CUP2012_1A_NEXTROUND_HPP
