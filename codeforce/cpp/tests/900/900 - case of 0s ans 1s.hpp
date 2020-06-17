#ifndef CODEFORCE_CPP__900_CASEOF0SANS1S_HPP
#define CODEFORCE_CPP__900_CASEOF0SANS1S_HPP
#define TESTNAME __

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <iostream>
#include <deque>



TMAIN()
{
	auto n = 0;
	std::string s;
	std::cin >> n >> s;

	auto letters = std::deque<char>();
	for (auto c : s)
	{
		if (!letters.size())
		{
			letters.push_back(c);
			continue;
		}

		auto t = letters.back();
		if (t != c)
		{
			letters.pop_back();
		}
		else
		{
			letters.push_back(c);
		}
	}

	std::cout << letters.size() << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__900_CASEOF0SANS1S_HPP
