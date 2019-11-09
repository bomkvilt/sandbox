#include <gtest/gtest.h>
#include "0 - swap of letters.hpp"


struct SwapOfLettersTests : public testing::Test
{
};


TEST_F(SwapOfLettersTests, t1)
{
	auto [swaps, ok] = SwapOfLetters::Swap("abab", "aabb");
	ASSERT_EQ(ok, true);
	ASSERT_EQ(swaps, SwapOfLetters::Swaps({
		{ 2, 2}, { 2, 1 }
	}));
}