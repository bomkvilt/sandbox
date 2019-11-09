#ifndef CODEFORCE_CPP_500_ISTHETASKEASY_HPP
#define CODEFORCE_CPP_500_ISTHETASKEASY_HPP

#include <vector>



class IsTheTaskEasy
{
public:
	static bool IsEasy(const std::vector<int>& votes)
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
};


#if 0
#include <iostream>
int main()
{
	int n = 0;
	std::cin >> n;

	std::vector<int> buffer;
	buffer.resize(n);
	for (int i = 0; i < n; ++i)
	{
		std::cin >> buffer[i];
	}

	std::cout << (IsTheTaskEasy::IsEasy(buffer)
		? "EASY"
		: "HARD"
		) << std::endl;
}
#endif //!CODEFORCE_CPP_500_ISTHETASKEASY_HPP
#endif //!CODEFORCE_CPP_500_ISTHETASKEASY_HPP
