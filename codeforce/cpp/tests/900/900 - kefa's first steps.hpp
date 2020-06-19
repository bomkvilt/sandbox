#ifndef CODEFORCE_CPP__900_KEFA_SFIRSTSTEPS_HPP
#define CODEFORCE_CPP__900_KEFA_SFIRSTSTEPS_HPP
#define TESTNAME KEFA_SFIRSTSTEPS

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
	auto s = 0;
	auto m = 0;
	while (n--)
	{
		auto b = 0;
		std::cin >> b;

		if (b < a)
		{
			m = s > m ? s : m;
			s = 1;
		}
		else ++s;

		a = b;
	}
	m = s > m ? s : m;

	std::cout << m << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__900_KEFA_SFIRSTSTEPS_HPP
