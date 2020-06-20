#include <gtest/gtest.h>
#include "math.mpow.hpp"


TEST(tests, math_mpow)
{
	auto cases = std::vector<std::tuple<int, int, int, int, int>>{
		  {1, 0, 1,  10,  0}
		, {2, 2, 2,  10,  4}
		, {3, 2, 3,  10,  8}
		, {4, 2, 3,   3,  2}
		, {5, 9, 9, 267, 18}
		, {6, 361375589, 56745, 998244353, 735420370}
	};

	for (auto [N, x, y, m, exp] : cases)
	{
		auto res = MPow(x, y, m);
		EXPECT_EQ(res, exp) << "N = " << N;
	}
}
