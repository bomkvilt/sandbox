#ifndef CODEFORCE_CPP__900_637A_NASTYAANDRIZ_HPP
#define CODEFORCE_CPP__900_637A_NASTYAANDRIZ_HPP
#define PROBLEM_NAME NastyaAndRiz
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	auto t = 0;
	std::cin >> t;

	while (t--)
	{
		int n, a, b, c, d;
		std::cin >> n >> a >> b >> c >> d;

		auto r0 = (a - b) * n;
		auto r1 = (a + b) * n;
		auto p0 = c - d;
		auto p1 = c + d;
		
		std::cout << (r1 >= p0 && r0 <= p1 ? "YES" : "NO") << std::endl;
	}
	return 0;
}


#endif //!CODEFORCE_CPP__900_637A_NASTYAANDRIZ_HPP
