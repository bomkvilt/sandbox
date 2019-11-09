#include <gtest/gtest.h>
#include "600 - two bears.hpp"


struct TwoBearsTests : public testing::Test
{
};


TEST_F(TwoBearsTests, t1)
{
	auto res = TwoBears::GetTime(4, 7);
	ASSERT_EQ(res, 2);
}

TEST_F(TwoBearsTests, t2)
{
	auto res = TwoBears::GetTime(4, 9);
	ASSERT_EQ(res, 3);
}

TEST_F(TwoBearsTests, t3)
{
	auto res = TwoBears::GetTime(1, 1);
	ASSERT_EQ(res, 1);
}
