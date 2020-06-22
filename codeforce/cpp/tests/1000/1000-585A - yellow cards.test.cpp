#include "cf.test.hpp"
#include "1000-585A - yellow cards.hpp"



TEST(tests, PROBLEM_NAME)
{
	{
		auto t = PROBLEM_NAME(2, 3, 5, 1, 8);
		auto [min, max] = t.GetMinMax();
		ASSERT_EQ(min, 0);
		ASSERT_EQ(max, 4);
	}
	{
		auto t = PROBLEM_NAME(3, 1, 6, 7, 25);
		auto [min, max] = t.GetMinMax();
		ASSERT_EQ(min, 4);
		ASSERT_EQ(max, 4);
	}
	{
		auto t = PROBLEM_NAME(6, 4, 9, 10, 89);
		auto [min, max] = t.GetMinMax();
		ASSERT_EQ(min, 5);
		ASSERT_EQ(max, 9);
	}
}
