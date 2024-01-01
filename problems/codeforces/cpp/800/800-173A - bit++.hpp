#ifndef CODEFORCE_CPP__800_173A_BIT___HPP
#define CODEFORCE_CPP__800_173A_BIT___HPP
#define PROBLEM_NAME Bit
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__800_173A_BIT___HPP
