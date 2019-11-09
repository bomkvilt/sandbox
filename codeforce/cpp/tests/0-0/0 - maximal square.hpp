#ifndef CODEFORCE_CPP_0_MAXIMALSQUARE_HPP
#define CODEFORCE_CPP_0_MAXIMALSQUARE_HPP

#include <vector>
#include <algorithm>


// https://codeforces.com/problemset/problem/1243/A
class MaximalSquare
{
public:
	static int FindMaximalSquare(std::vector<int> boards)
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
};


#if 0
#include <iostream>
int main()
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
		
		auto res = MaximalSquare::FindMaximalSquare(buff);
		std::cout << res << std::endl;
	}
}
#endif //!CODEFORCE_CPP_0_MAXIMALSQUARE_HPP
#endif //!CODEFORCE_CPP_0_MAXIMALSQUARE_HPP
