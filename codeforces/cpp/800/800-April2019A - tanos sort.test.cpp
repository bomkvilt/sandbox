#include "cf.test.hpp"
#include "800-April2019A - tanos sort.hpp"



TEST(tests, PROBLEM_NAME)
{
	{
		auto len = PROBLEM_NAME::GetMaxLen({ 1, 2, 2, 4 });
		ASSERT_EQ(len, 4);
	}
	{
		auto len = PROBLEM_NAME::GetMaxLen({ 11, 12, 1, 2, 13, 14, 3, 4 });
		ASSERT_EQ(len, 2);
	}
	{
		auto len = PROBLEM_NAME::GetMaxLen({ 11, 12, 1, 2, 13, 14, 15, 16 });
		ASSERT_EQ(len, 4);
	}
	{
		auto len = PROBLEM_NAME::GetMaxLen({ 7, 6, 5, 4 });
		ASSERT_EQ(len, 1);
	}
}
