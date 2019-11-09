#include <gtest/gtest.h>
#include "600 - halk.hpp"


struct HalkTests : public testing::Test
{
};


TEST_F(HalkTests, t1)
{
	auto res = Halk::GetFillings(1);
	ASSERT_EQ(res, "I hate it");
}

TEST_F(HalkTests, t2)
{
	auto res = Halk::GetFillings(2);
	ASSERT_EQ(res, "I hate that I love it");
}

TEST_F(HalkTests, t3)
{
	auto res = Halk::GetFillings(3);
	ASSERT_EQ(res, "I hate that I love that I hate it");
}
