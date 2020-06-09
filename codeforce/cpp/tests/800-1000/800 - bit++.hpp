#ifndef CODEFORCE_CPP__800_BIT++_HPP
#define CODEFORCE_CPP__800_BIT++_HPP
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

	auto m = 0;
	auto command = std::string();
	while (n--)
	{
		std::cin >> command;
		if (command.find("+") < command.length())
		{
			++m;
		}
		else
		{
			--m;
		}
	}
	std::cout << m << std::endl;
	return 0;
}

#endif //!CODEFORCE_CPP__800_BIT++_HPP
