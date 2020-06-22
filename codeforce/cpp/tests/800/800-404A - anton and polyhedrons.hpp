#ifndef CODEFORCE_CPP__800_404A_ANTONANDPOLYHEDRONS_HPP
#define CODEFORCE_CPP__800_404A_ANTONANDPOLYHEDRONS_HPP
#define PROBLEM_NAME AntonAndPolyhedrons
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	int n = 0;
	std::cin >> n;
	
	auto summ = 0;
	auto buff = std::string();
	while (n--)
	{
		std::cin >> buff;
		switch (buff[0]) {
		case 'T': summ += 4;  break;
		case 'C': summ += 6;  break;
		case 'O': summ += 8;  break;
		case 'D': summ += 12; break;
		case 'I': summ += 20; break;
		}
	}
	std::cout << summ << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_404A_ANTONANDPOLYHEDRONS_HPP
