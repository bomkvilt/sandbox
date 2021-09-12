#ifndef CODEFORCE_CPP__900_130A_DUBSTEP_HPP
#define CODEFORCE_CPP__900_130A_DUBSTEP_HPP
#define PROBLEM_NAME Dubstep
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_130A_DUBSTEP_HPP
