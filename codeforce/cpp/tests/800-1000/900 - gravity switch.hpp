#ifndef CODEFORCE_CPP__900_GRAVITYSWITCH_HPP
#define CODEFORCE_CPP__900_GRAVITYSWITCH_HPP
#define TESTNAME __

#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(TESTNAME)
#endif

#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>



TMAIN()
{
	auto n = 0;
	std::cin >> n;

	auto data = std::vector<int>(n, 0);
	while (n--)
	{
		std::cin >> data[n];
	}
	std::sort(data.begin(), data.end());

	for (auto v : data)
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	return 0;
}


#endif //!CODEFORCE_CPP__900_GRAVITYSWITCH_HPP
