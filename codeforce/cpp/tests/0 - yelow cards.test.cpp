#include <gtest/gtest.h>
#include "0 - yelow cards.hpp"


struct YellowCardsTests : public testing::Test 
{};


TEST_F(YellowCardsTests, t1)
{
	auto t = YellowCards(2, 3, 5, 1, 8);
	auto [min, max] = t.GetMinMax();
	ASSERT_EQ(min, 0);
	ASSERT_EQ(max, 4);
}

TEST_F(YellowCardsTests, t2)
{
	auto t = YellowCards(3, 1, 6, 7, 25);
	auto [min, max] = t.GetMinMax();
	ASSERT_EQ(min, 4);
	ASSERT_EQ(max, 4);
}

TEST_F(YellowCardsTests, t3)
{
	auto t = YellowCards(6, 4, 9, 10, 89);
	auto [min, max] = t.GetMinMax();
	ASSERT_EQ(min, 5);
	ASSERT_EQ(max, 9);
}
