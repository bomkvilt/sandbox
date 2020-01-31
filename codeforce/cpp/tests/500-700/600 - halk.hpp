#ifndef CODEFORCE_CPP__600_HALK_HPP
#define CODEFORCE_CPP__600_HALK_HPP

#include <string>


class Halk
{
public:
	static std::string GetFillings(int lvl)
	{
		static const std::string pair = "that I love that I hate ";
		static const std::string love = "that I love ";
		static const std::string hate = "I hate ";

		auto result = std::string(hate);
		if (int pairs = (lvl - 1) / 2)
		{
			result.reserve(result.length() + pairs * pair.length());
			while (pairs--)
			{
				result += pair;
			}
		}
		if ((lvl - 1) % 2)
		{
			result += love;
		}
		return result + "it";
	}
};


#if 0
#include <iostream>
int main()
{
	int n = 0;
	std::cin >> n;
	std::cout << Halk::GetFillings(n) << std::endl;
}
#endif //!CODEFORCE_CPP_600_HALK_HPP
#endif //!CODEFORCE_CPP__600_HALK_HPP
