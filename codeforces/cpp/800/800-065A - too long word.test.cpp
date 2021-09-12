#include "cf.test.hpp"
#include "800-065A - too long word.hpp"



TEST(tests, PROBLEM_NAME)
{
	auto cases = std::vector<std::pair<std::string, std::string>>
	{
		  {"word", "word"}
		, {"localization", "l10n"}
		, {"internationalization", "i18n"}
		, {"pneumonoultramicroscopicsilicovolcanoconiosis" , "p43s"}
	};

	for (auto [w, exp] : cases)
	{
		auto res = TESTNAME(w);
		EXPECT_EQ(res, exp) << "w = " << w;
	}
}
