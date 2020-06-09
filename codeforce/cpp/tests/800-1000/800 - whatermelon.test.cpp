#include "cf.test.hpp"
#include "800 - whatermelon.hpp"



TEST(tests, Whatermellon)
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
		auto res = Whatermelon::CanBeSplit(w);
		EXPECT_EQ(res, exp) << "w = " << w;
	}
}
