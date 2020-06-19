#ifndef CODEFORCE_CPP__900_HQ9__HPP
#define CODEFORCE_CPP__900_HQ9__HPP
#define TESTNAME HQ9P

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>



TMAIN()
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
}

#endif //!CODEFORCE_CPP__900_HQ9__HPP
