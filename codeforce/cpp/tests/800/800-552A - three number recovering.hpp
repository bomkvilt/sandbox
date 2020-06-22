#ifndef CODEFORCE_CPP__800_552A_THREENUMBERRECOVERING_HPP
#define CODEFORCE_CPP__800_552A_THREENUMBERRECOVERING_HPP
#define PROBLEM_NAME ThreeNumberRecovering
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



auto PROBLEM_NAME(int ab, int ac, int abc) -> std::tuple<int, int, int>
{
	return { ab - abc + ac, abc - ac, abc - ab };
}


PROBLEM_MAIN()
{
	std::array<int, 4> v;
	for (int i = 0; i < 4; ++i)
	{
		std::cin >> v[i];
	}
	std::sort(v.begin(), v.end());

	auto [a, b, c] = PROBLEM_NAME(v[0], v[1], v[3]);
	std::cout << a << " " << b << " " << c << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_552A_THREENUMBERRECOVERING_HPP
