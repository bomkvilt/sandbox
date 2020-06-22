#ifndef CODEFORCE_CPP__800_CUP2019_1A_SEARCHINGOFSIMPLEPROBLEMS_HPP
#define CODEFORCE_CPP__800_CUP2019_1A_SEARCHINGOFSIMPLEPROBLEMS_HPP
#define PROBLEM_NAME SearchingOfSimpleProblems
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



bool PROBLEM_NAME(const std::vector<int>& votes)
{
	for (auto vote : votes)
	{
		if (vote)
		{
			return false;
		}
	}
	return true;
}


PROBLEM_MAIN()
{
	int n = 0;
	std::cin >> n;

	std::vector<int> buffer;
	buffer.resize(n);
	for (int i = 0; i < n; ++i)
	{
		std::cin >> buffer[i];
	}
	
	std::cout << (PROBLEM_NAME(buffer) ? "EASY" : "HARD" ) << std::endl;

	return 0;
}


#endif //!CODEFORCE_CPP__800_CUP2019_1A_SEARCHINGOFSIMPLEPROBLEMS_HPP
