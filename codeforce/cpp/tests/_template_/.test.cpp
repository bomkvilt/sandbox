#include "cf.test.hpp"
#include ""



TEST(tests, TESTNAME)
{
	auto cases = std::vector<std::pair<, >>
	{
		  {, }
		, {, }
	};

	for (auto [w, exp] : cases)
	{
		auto res = TESTNAME(w);
		EXPECT_EQ(res, exp) << "w = " << w;
	}
}
