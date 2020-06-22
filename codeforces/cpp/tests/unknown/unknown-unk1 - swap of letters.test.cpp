#include "cf.test.hpp"
#include "unknown-unk1 - swap of letters.hpp"



TEST(tests, PROBLEM_NAME)
{
	auto exp = PROBLEM_NAME::Swaps({{ 2, 2}, { 2, 1 }});
	auto [res, ok] = PROBLEM_NAME::Swap("abab", "aabb");
	ASSERT_EQ(ok, true);
	ASSERT_EQ(res, exp);
}
