#include <gtest/gtest.h>
#include "500 - incorrect substruction.hpp"


struct IncorrectSubstructionTests : public testing::Test
{
};


TEST_F(IncorrectSubstructionTests, t1)
{
	auto res = IncorrectSubstruction::Substruct(512, 4);
	ASSERT_EQ(res, 50);
}

TEST_F(IncorrectSubstructionTests, t2)
{
	auto res = IncorrectSubstruction::Substruct(1000000000, 9);
	ASSERT_EQ(res, 1);
}
