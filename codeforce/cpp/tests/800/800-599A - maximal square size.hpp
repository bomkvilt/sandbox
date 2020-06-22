#ifndef CODEFORCE_CPP__800_599A_MAXIMALSQUARESIZE_HPP
#define CODEFORCE_CPP__800_599A_MAXIMALSQUARESIZE_HPP
#define PROBLEM_NAME MaximalSquareSize
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <vector>
#include <algorithm>



int PROBLEM_NAME(std::vector<int> boards)
{
	std::sort(boards.begin(), boards.end());
	
	int n = 0;
	auto itr = boards.rbegin();
	for (; n < boards.size(); ++n)
	{
		if (itr[n] < n + 1)
		{
			return n;
		}
	}
	return n;
}


PROBLEM_MAIN()
{
	int n = 0;
	std::cin >> n;

	std::vector<int> buff;
	while (n--)
	{
		int p = 0;
		std::cin >> p;
		buff.resize(p);
		for (int i = 0; i < p; ++i)
		{
			std::cin >> buff[i];
		}
		
		auto res = PROBLEM_NAME(buff);
		std::cout << res << std::endl;
	}
	return 0;
}


#endif //!CODEFORCE_CPP__800_599A_MAXIMALSQUARESIZE_HPP
