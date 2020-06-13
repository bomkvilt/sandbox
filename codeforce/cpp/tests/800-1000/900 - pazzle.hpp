#ifndef CODEFORCE_CPP__900_PAZZLE_HPP
#define CODEFORCE_CPP__900_PAZZLE_HPP
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
	auto m = 0;
	std::cin >> n >> m;
	
	auto data = std::vector<int>(m);
	for (auto i = 0; i < m; ++i)
	{
		std::cin >> data[i];
	}
	std::sort(data.begin(), data.end(), std::greater<int>());

	auto d = INT32_MAX;
	for (auto i = 0; i <= m - n; ++i)
	{
		auto diff = data[i] - data[i + n - 1];
		d = d < diff ? d : diff;
	}

	std::cout << d << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__900_PAZZLE_HPP
