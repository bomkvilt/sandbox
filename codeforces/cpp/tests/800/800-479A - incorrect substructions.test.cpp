#include "cf.test.hpp"
#include "800-479A - incorrect substructions.hpp"



TEST(tests, PROBLEM_NAME)
{
	auto cases = std::vector<std::tuple<int, int, int, int>>
	{
		  {1, 512, 4, 50}
		, {2, 1000000000, 9, 1}
	};
	
	for (auto [N, n, k, exp] : cases)
	{
		auto res = PROBLEM_NAME(n, k);
		EXPECT_EQ(res, exp) << "N = " << N;
	}
}
