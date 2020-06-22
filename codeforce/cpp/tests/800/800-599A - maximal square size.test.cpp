#include "cf.test.hpp"
#include "800-599A - maximal square size.hpp"



TEST(tests, PROBLEM_NAME)
{
	auto cases = std::vector<std::tuple<int, std::vector<int>, int>>
	{
		  {1, {4, 3, 1 ,4 ,5 }, 3}
		, {2, {4, 4, 4 ,4 }, 4}
		, {3, {1, 1, 1 }, 1}
		, {4, {5, 5, 1, 1, 5 }, 3}
	};
	
	for (auto [N, v, exp] : cases)
	{
		auto res = PROBLEM_NAME(v);
		EXPECT_EQ(res, exp) << "N = " << N;
	}
}
