#include "gtest/gtest.h"

#include "reflect/archived.hpp"
#include "reflect/archiver.hpp"
#include <boost/algorithm/string/replace.hpp>


struct archiver_tests : public testing::Test
{
	reflect::Archiver archive;

	void RemoveSpace(std::string& str)
	{
		boost::replace_all(str, " ", "");
		boost::replace_all(str, "\t", "");
		boost::replace_all(str, "\n", "");
	}

	template<typename T>
	std::string Probe(T& probe, const std::string& name)
	{
		archive.AddField(probe, name);
		auto json = archive.Save();
		RemoveSpace(json);
		return json;
	}
};


TEST_F(archiver_tests, simple_load)
{
	archive.Load( R"({"a": "gg"})");
	auto probe = std::string();
	archive.GetField(probe, "a");
	EXPECT_EQ(probe, "gg");
}

TEST_F(archiver_tests, simple_write)
{
	auto probe = std::string("wp");
	auto json = Probe(probe, "aa");
	EXPECT_EQ(json, R"({"aa":"wp"})");
}

TEST_F(archiver_tests, simple_int)
{
	auto probe = int(10);
	auto json = Probe(probe, "aa");
	EXPECT_EQ(json, R"({"aa":"10"})");

	auto result = int(0);
	archive.GetField(result, "aa");
	EXPECT_EQ(result, probe);
}

TEST_F(archiver_tests, simple_bool)
{
	auto probe = true;
	auto json = Probe(probe, "aa");
	EXPECT_EQ(json, R"({"aa":"true"})");

	auto result = false;
	archive.GetField(result, "aa");
	EXPECT_EQ(result, probe);
}

TEST_F(archiver_tests, simple_float)
{
	auto probe = float(2.5);
	auto json = Probe(probe, "aa");
	EXPECT_EQ(json, R"({"aa":"2.5"})");

	auto result = float(0);
	archive.GetField(result, "aa");
	EXPECT_EQ(result, probe);
}

TEST_F(archiver_tests, complex_name)
{
	auto probe = std::string("ez");
	auto json = Probe(probe, "aa.b");
	EXPECT_EQ(json, R"({"aa":{"b":"ez"}})");

	auto result = std::string("");
	archive.GetField(result, "aa.b");
	EXPECT_EQ(result, probe);
}

TEST_F(archiver_tests, unquotted)
{
	archive.Load(R"({"aa":10})");
	auto probe = int(0);
	archive.GetField(probe, "aa");
	EXPECT_EQ(probe, 10);
}

TEST_F(archiver_tests, array)
{
	auto probe = std::vector<int>{10, 20};
	auto json = Probe(probe, "aa");
	EXPECT_EQ(json, R"({"aa":["10","20"]})");

	auto result = std::vector<int>();
	archive.GetField(result, "aa");
	EXPECT_EQ(result, probe);
}

TEST_F(archiver_tests, map)
{
	auto probe = std::map<int, std::string>{ { 10, "a"}, {20, "b"} };
	auto json = Probe(probe, "aa");
	EXPECT_EQ(json, R"({"aa":{"10":"a","20":"b"}})");

	auto result = std::map<int, std::string>();
	archive.GetField(result, "aa");
	EXPECT_EQ(result, probe);
}
