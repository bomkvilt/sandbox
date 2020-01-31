#ifndef CODEFORCE_CPP__600_THREENUMS_HPP
#define CODEFORCE_CPP__600_THREENUMS_HPP


#include <tuple>
#include <array>
#include <algorithm>



class ThreeNums
{
public:
	static auto Recover(int ab, int ac, int abc) -> std::tuple<int, int, int>
	{
		return { ab - abc + ac, abc - ac, abc - ab };
	}
};


#if 0
#include <iostream>
int main()
{
	std::array<int, 4> v;
	for (int i = 0; i < 4; ++i)
	{
		std::cin >> v[i];
	}
	std::sort(v.begin(), v.end());

	auto [a, b, c] = ThreeNums::Recover(v[0], v[1], v[3]);
	std::cout << a << " " << b << " " << c << std::endl;
}
#endif //!CODEFORCE_CPP_600_THREENUMS_HPP
#endif //!CODEFORCE_CPP__600_THREENUMS_HPP
