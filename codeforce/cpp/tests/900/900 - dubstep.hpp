#ifndef CODEFORCE_CPP__900_DUBSTEP_HPP
#define CODEFORCE_CPP__900_DUBSTEP_HPP
#define TESTNAME DUBSTEP

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>



TMAIN()
{
	std::string text;
	std::cin >> text;

    size_t pos = 0;
    while (true) {
        pos = text.find("WUB", pos);
        if (pos == std::string::npos)
        {
            break;
        }

        text.replace(pos, 3, " ");
        pos += 1;
    }
    std::cout << text << std::endl;

	return 0;
}

#endif //!CODEFORCE_CPP__900_DUBSTEP_HPP
