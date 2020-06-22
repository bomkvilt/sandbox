#ifndef CODEFORCE_CPP__900_310A_CASEOFZEROSANDUNITS_HPP
#define CODEFORCE_CPP__900_310A_CASEOFZEROSANDUNITS_HPP
#define PROBLEM_NAME CaseOfZerosAndUnits
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_310A_CASEOFZEROSANDUNITS_HPP
