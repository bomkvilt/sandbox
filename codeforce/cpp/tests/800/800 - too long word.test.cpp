#include "cf.test.hpp"
#include "800 - too long word.hpp"



TEST(tests, TESTNAME)
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
