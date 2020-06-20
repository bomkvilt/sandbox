#include "cf.test.hpp"
#include "2100 - all heights are the same.hpp"



TEST(tests, TESTNAME)
{
	auto cases = std::vector<std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>>{
		// N, n, m, L, R, exp
		  {1, 2, 2, 1, 1, 1}
		, {2, 1, 2, 1, 2, 2}
		, {3, 485, 117, 386829368, 748204956, 735420370}
		, {4, 564, 558, 305171115, 960941497, 880111542}
		, {5, 692, 210,  44175861, 843331069, 714028205}
		, {9,  13, 635, 761278633, 941090619, 893955177}
	};

	for (auto [N, n, m, L, R, exp] : cases)
	{
		auto res = TESTNAME(n, m, L, R);
		EXPECT_EQ(res, exp) << "N = " << N;
	}
}
