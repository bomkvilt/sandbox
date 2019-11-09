#include "gtest/gtest.h"
#include "config/config.hpp"
#include <filesystem>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>


struct config_tests : public testing::Test
{
	void SetUp() override
	{
		if (std::filesystem::exists(path))
		{
			std::filesystem::remove(path);
		}
	}

	void TearDown() override
	{
		if (std::filesystem::exists(path))
		{
			std::filesystem::remove(path);
		}
	}

	void RemoveSpace(std::string& str)
	{
		boost::replace_all(str, " ", "");
		boost::replace_all(str, "\t", "");
		boost::replace_all(str, "\n", "");
	}

	std::filesystem::path path = "./___config_test.json";
};

TEST_F(config_tests, simple)
{
	struct Probe : public reflect::FConfig
	{
		ARCH_BEGIN(reflect::FConfig)
			ARCH_FIELD( , ,a)
			ARCH_END();
		int a = 10;
	} probe, result;

	EXPECT_TRUE(probe.SaveConfig(path.string()));
	
	auto file = std::ifstream(path);
	EXPECT_TRUE(file);
	auto json = std::string(std::istreambuf_iterator(file), {});
	RemoveSpace(json);
	EXPECT_EQ(json, R"({"a":"10"})");

	result.a = 0;
	EXPECT_TRUE(result.LoadConfig(path.string()));
	EXPECT_EQ(result.a, probe.a);
}
