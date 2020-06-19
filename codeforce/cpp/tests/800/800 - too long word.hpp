#ifndef CODEFORCE_CPP__800_TOOLONGWORD_HPP
#define CODEFORCE_CPP__800_TOOLONGWORD_HPP
#define TESTNAME TOOLONGWORD

#ifndef TMAIN
#define TMAIN int main
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



TMAIN()
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


#endif //!CODEFORCE_CPP__800_TOOLONGWORD_HPP
