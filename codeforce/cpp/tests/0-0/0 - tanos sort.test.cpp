#include <gtest/gtest.h>
#include "0 - tanos sort.hpp"


struct TanosSortTests : public testing::Test
{};


TEST_F(TanosSortTests, t1)
{
	auto len = TanosSort::GetMaxLen({ 1, 2, 2, 4 });
	ASSERT_EQ(len, 4);
}

TEST_F(TanosSortTests, t2)
{
	auto len = TanosSort::GetMaxLen({ 11, 12, 1, 2, 13, 14, 3, 4 });
	ASSERT_EQ(len, 2);
}

TEST_F(TanosSortTests, t3)
{
	auto len = TanosSort::GetMaxLen({ 11, 12, 1, 2, 13, 14, 15, 16 });
	ASSERT_EQ(len, 4);
}

TEST_F(TanosSortTests, t4)
{
	auto len = TanosSort::GetMaxLen({ 7, 6, 5, 4 });
	ASSERT_EQ(len, 1);
}
