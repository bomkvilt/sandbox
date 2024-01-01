#ifndef CODEFORCE_CPP__900_238A_GRAVITYSWITCH_HPP
#define CODEFORCE_CPP__900_238A_GRAVITYSWITCH_HPP
#define PROBLEM_NAME GravitySwitch
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>



PROBLEM_MAIN()
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


#endif //!CODEFORCE_CPP__900_238A_GRAVITYSWITCH_HPP
