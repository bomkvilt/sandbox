#ifndef CODEFORCE_CPP__900_321A_KEFAANDFIRSTSTEPS_HPP
#define CODEFORCE_CPP__900_321A_KEFAANDFIRSTSTEPS_HPP
#define PROBLEM_NAME KefaAndFirstSteps
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_321A_KEFAANDFIRSTSTEPS_HPP
