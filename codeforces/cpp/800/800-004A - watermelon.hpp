#ifndef CODEFORCE_CPP__800_004A_WATERMELON_HPP
#define CODEFORCE_CPP__800_004A_WATERMELON_HPP
#define PROBLEM_NAME Watermelon
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



bool PROBLEM_NAME(int w)
{
	return w > 2 && !(w % 2);
}


PROBLEM_MAIN()
{
	int w = 0;
	std::cin >> w;
	std::cout << (PROBLEM_NAME(w) ? "YES" : "NO") << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_004A_WATERMELON_HPP
