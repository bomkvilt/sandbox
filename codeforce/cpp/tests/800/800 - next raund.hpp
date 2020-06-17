#ifndef CODEFORCE_CPP__800_NEXTRAUND_HPP
#define CODEFORCE_CPP__800_NEXTRAUND_HPP
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

#endif //!CODEFORCE_CPP__800_NEXTRAUND_HPP
