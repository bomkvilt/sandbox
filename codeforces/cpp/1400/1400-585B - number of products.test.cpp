#include "cf.test.hpp"
#include "1400-585B - number of products.hpp"



TEST(tests, PROBLEM_NAME)
{
	auto cases = std::vector<std::tuple<int, std::vector<int>, std::tuple<int, int>>>
	{
		  {1, {5, -3, 3, -1, 1}, {8, 7}}
		, {2, {4, 2, -4, 3, 1, 2, -4, 3, 2, 3}, {28, 27}}
		, {3, {-1, -2, -3, -4, -5}, {9, 6}}
	};
	
	for (auto [N, v, exp] : cases)
	{
		auto [neg_exp, pos_exp] = exp;
		auto [neg_res, pos_res] = PROBLEM_NAME(v);
		EXPECT_EQ(neg_res, neg_exp) << "N = " << N;
		EXPECT_EQ(pos_res, pos_exp) << "N = " << N;
	}
}
