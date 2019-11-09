#include "gtest/gtest.h"

#include "reflect/archived.hpp"
#include "reflect/archiver.hpp"
#include <boost/algorithm/string/replace.hpp>


struct archived_tests : public testing::Test
{
	reflect::Archiver archive;

	void RemoveSpace(std::string& str)
	{
		boost::replace_all(str, " ", "");
		boost::replace_all(str, "\t", "");
		boost::replace_all(str, "\n", "");
	}
};


TEST_F(archived_tests, simple)
{
	struct FTest : public reflect::FArchived
	{
		ARCH_BEGIN(reflect::FArchived)
			ARCH_FIELD(,, probe) 
			ARCH_END();
		int probe = 86;
	} a; 
	a.Marshal(archive);
	auto json = archive.Save();
	RemoveSpace(json);
	EXPECT_EQ(json, R"({"probe":"86"})");
}

TEST_F(archived_tests, category)
{
	struct FTest : public reflect::FArchived
	{
		ARCH_BEGIN(reflect::FArchived)
			ARCH_FIELD(aa,, probe)
			ARCH_END();
		int probe = 86;
	} a;
	a.Marshal(archive);
	auto json = archive.Save();
	RemoveSpace(json);
	EXPECT_EQ(json, R"({"aa":{"probe":"86"}})");
}

TEST_F(archived_tests, alt_name)
{
	struct FTest : public reflect::FArchived
	{
		ARCH_BEGIN(reflect::FArchived)
			ARCH_FIELD(,aa, probe)
			ARCH_END();
		int probe = 86;
	} a;
	a.Marshal(archive);
	auto json = archive.Save();
	RemoveSpace(json);
	EXPECT_EQ(json, R"({"aa":"86"})");
}

TEST_F(archived_tests, array)
{
	struct FTest : public reflect::FArchived
	{
		ARCH_BEGIN(reflect::FArchived)
			ARCH_FIELD(,, probe)
			ARCH_END();
		std::vector<int> probe = { 86, 64 };
	} a, b;
	a.Marshal(archive);
	auto json = archive.Save();
	RemoveSpace(json);
	EXPECT_EQ(json, R"({"probe":["86","64"]})");

	b.probe.clear();
	b.Unmarshal(archive);
	EXPECT_EQ(b.probe, a.probe);
}

TEST_F(archived_tests, archived)
{
	struct FTest : public reflect::FArchived
	{
		ARCH_BEGIN(reflect::FArchived)
			ARCH_FIELD(,, probe)
			ARCH_END();
		struct FEntery : public reflect::FArchived
		{
			ARCH_BEGIN(reflect::FArchived)
				ARCH_FIELD(,, probe)
				ARCH_END();
			int probe = 85;
		} probe;
	} a, b;
	a.Marshal(archive);
	auto json = archive.Save();
	RemoveSpace(json);
	EXPECT_EQ(json, R"({"probe":{"probe":"85"}})");

	b.probe.probe = 0;
	b.Unmarshal(archive);
	EXPECT_EQ(b.probe.probe, a.probe.probe);
}
