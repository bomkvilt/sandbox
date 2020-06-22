#ifndef CODEFORCE_CPP__800_405A_MISHKAANDHISELDERBROTHER_HPP
#define CODEFORCE_CPP__800_405A_MISHKAANDHISELDERBROTHER_HPP
#define PROBLEM_NAME MishkaAndHisElderBrother
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <cmath>



PROBLEM_MAIN()
{
	int a = 0;
	std::cin >> a;

	int b = 0;
	std::cin >> b;

	std::cout << int(std::log((double)b / a) / std::log(1.5)) + 1 << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_405A_MISHKAANDHISELDERBROTHER_HPP
