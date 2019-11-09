#include <gtest/gtest.h>
#include "0 - maximal square.hpp"


struct MaximalSquareTests : public testing::Test
{
};


TEST_F(MaximalSquareTests, t1)
{
	auto res = MaximalSquare::FindMaximalSquare({ 4 ,3 ,1 ,4 ,5 });
	ASSERT_EQ(res, 3);
}

TEST_F(MaximalSquareTests, t2)
{
	auto res = MaximalSquare::FindMaximalSquare({ 4 ,4 ,4 ,4 });
	ASSERT_EQ(res, 4);
}

TEST_F(MaximalSquareTests, t3)
{
	auto res = MaximalSquare::FindMaximalSquare({ 1, 1, 1 });
	ASSERT_EQ(res, 1);
}

TEST_F(MaximalSquareTests, t4)
{
	auto res = MaximalSquare::FindMaximalSquare({ 5, 5, 1, 1, 5 });
	ASSERT_EQ(res, 3);
}
