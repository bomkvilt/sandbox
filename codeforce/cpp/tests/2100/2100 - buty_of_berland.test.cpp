#include "cf.test.hpp"
#include "2100 - buty_of_berland.hpp"



TEST(tests, TESTNAME)
{
	{
		auto scheme = std::vector{ std::pair{1, 2}, {3, 2}, {3, 4} };
		auto votes = std::vector{ std::tuple{1, 2, 5}, {1, 3, 3} };
		auto exp = std::vector{5, 3, 5};

		auto test = ButyOfBerland();
		for (auto link : scheme)
		{
			test.AddLink(link.first, link.second);
		}
		test.ApplyMap();

		for (auto [a, b, vote] : votes)
		{
			auto ok = test.AddVote(a, b, vote);
			EXPECT_TRUE(ok);
		}
		ASSERT_TRUE(test.CheckConsistency());

		auto res = test.GetButy();
		ASSERT_EQ(res, exp);
	}
	{
		auto scheme = std::vector{ std::pair{1, 2}, {1, 6}, {3, 1}, {1, 5}, {4, 1} };
		auto votes = std::vector{ std::tuple{6, 1, 3}, {3, 4, 1}, {6, 5, 2}, {1, 2, 5} };
		auto exp = std::vector{5, 3, 1, 2, 1};

		auto test = ButyOfBerland();
		for (auto link : scheme)
		{
			test.AddLink(link.first, link.second);
		}
		test.ApplyMap();

		for (auto [a, b, vote] : votes)
		{
			auto ok = test.AddVote(a, b, vote);
			EXPECT_TRUE(ok);
		}
		ASSERT_TRUE(test.CheckConsistency());

		auto res = test.GetButy();
		ASSERT_EQ(res, exp);
	}
	{
		auto scheme = std::vector{ std::pair{1, 2}, {1, 6}, {3, 1}, {1, 5}, {4, 1} };
		auto votes = std::vector{ std::tuple{6, 1, 1}, {3, 4, 3}, {6, 5, 3}, {1, 2, 4} };

		auto test = ButyOfBerland();
		for (auto link : scheme)
		{
			test.AddLink(link.first, link.second);
		}
		test.ApplyMap();

		for (auto [a, b, vote] : votes)
		{
			test.AddVote(a, b, vote);
		}
		ASSERT_FALSE(test.CheckConsistency());
	}
}
