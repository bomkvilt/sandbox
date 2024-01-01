#ifndef CODEFORCE_CPP__900_096A_HQ9__HPP
#define CODEFORCE_CPP__900_096A_HQ9__HPP
#define PROBLEM_NAME HQ9
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	auto text = std::string();
	std::cin >> text;

	auto pos = text.rbegin();
	auto end = text.rend();
	for (; pos != end; ++pos)
	{
		switch (*pos) {
		case 'H':
		case 'Q':
		case '9':
			std::cout << "YES" << std::endl;
			return 0;
		}
	}
	std::cout << "NO" << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__900_096A_HQ9__HPP
