#ifndef CODEFORCE_CPP__600_POLYHEDRONS_HPP
#define CODEFORCE_CPP__600_POLYHEDRONS_HPP


#if 0
#include <iostream>
int main()
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
}
#endif //!CODEFORCE_CPP_600_POLYHEDRONS_HPP
#endif //!CODEFORCE_CPP__600_POLYHEDRONS_HPP
