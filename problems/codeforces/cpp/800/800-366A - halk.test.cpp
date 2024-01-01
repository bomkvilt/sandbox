#include "cf.test.hpp"
#include "800-366A - halk.hpp"



TEST(tests, PROBLEM_NAME)
{
	auto cases = std::vector<std::tuple<int, int, std::string>>
	{
		  {1, 1, "I hate it"}
		, {2, 3, "I hate that I love that I hate it"}
	};
	
	for (auto [N, v, exp] : cases)
	{
		auto res = PROBLEM_NAME(v);
		EXPECT_EQ(res, exp) << "N = " << N;
	}
}
