#ifndef CODEFORCE_CPP__800_065A_TOOLONGWORD_HPP
#define CODEFORCE_CPP__800_065A_TOOLONGWORD_HPP
#define PROBLEM_NAME TooLongWord
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <string>



std::string TESTNAME(const std::string& word)
{
	constexpr auto limit = 10;

	if (word.size() <= limit)
	{
		return word;
	}
	return (*word.begin()) + std::to_string(word.size() - 2) + (*word.rbegin());
}


PROBLEM_MAIN()
{
	int n = 0;
	std::cin >> n;

	auto word = std::string();
	while (n--)
	{
		std::cin >> word;
		std::cout << TESTNAME(word) << std::endl;
		word.clear();
	}
	return 0;
}


#endif //!CODEFORCE_CPP__800_065A_TOOLONGWORD_HPP
