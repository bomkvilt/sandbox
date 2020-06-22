#ifndef CODEFORCE_CPP__800_366A_HALK_HPP
#define CODEFORCE_CPP__800_366A_HALK_HPP
#define PROBLEM_NAME Halk
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <string>


std::string PROBLEM_NAME(int lvl)
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


PROBLEM_MAIN()
{
	int n = 0;
	std::cin >> n;
	std::cout << PROBLEM_NAME(n) << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_366A_HALK_HPP
