#include <gtest/gtest.h>
#include "0 - number of products.hpp"


struct NumOfProdsTests : public testing::Test
{
};


TEST_F(NumOfProdsTests, t1)
{
	auto [neg, pos] = NumOfProds::GetCounts({ 5, -3, 3, -1, 1 });
	ASSERT_EQ(neg, 8);
	ASSERT_EQ(pos, 7);
}

TEST_F(NumOfProdsTests, t2)
{
	auto [neg, pos] = NumOfProds::GetCounts({ 4, 2, -4, 3, 1, 2, -4, 3, 2, 3 });
	ASSERT_EQ(neg, 28);
	ASSERT_EQ(pos, 27);
}

TEST_F(NumOfProdsTests, t3)
{
	auto [neg, pos] = NumOfProds::GetCounts({ -1, -2, -3, -4, -5 });
	ASSERT_EQ(neg, 9);
	ASSERT_EQ(pos, 6);
}
