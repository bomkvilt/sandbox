#include "cf.test.hpp"
#include "800-004A - watermelon.hpp"



TEST(tests, PROBLEM_NAME)
{
	auto cases = std::vector<std::pair<int, bool>>
	{
		  {8 , true}
		, {10, true}
		, {11, false}
		, {3 , false}
	};

	for (auto [w, exp] : cases)
	{
		auto res = PROBLEM_NAME(w);
		EXPECT_EQ(res, exp) << "w = " << w;
	}
}
