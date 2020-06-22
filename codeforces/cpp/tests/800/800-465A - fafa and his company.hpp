#ifndef CODEFORCE_CPP__800_465A_FAFAANDHISCOMPANY_HPP
#define CODEFORCE_CPP__800_465A_FAFAANDHISCOMPANY_HPP
#define PROBLEM_NAME FafaAndHisCompany
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



PROBLEM_MAIN()
{
	int c = 0;
	std::cin >> c;

	// c = n (m + 1)
	// m = c / n - 1
	int count = 0;
	for (int n = 1; n <= c / 2; ++n)
	{
		if (c % n == 0)
		{
			++count;
		}
	}
	std::cout << count << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_465A_FAFAANDHISCOMPANY_HPP
