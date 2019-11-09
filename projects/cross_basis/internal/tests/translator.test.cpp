#include <gtest/gtest.h>
#include "translator.hpp"


struct TranslatorTests : public testing::Test
{
	template<typename T>
	void Compare(const TVector<T> a, const TVector<T> b)
	{
		ASSERT_DOUBLE_EQ(round(a.x * 1e3) / 1e3f, round(b.x * 1e3) / 1e3f);
		ASSERT_DOUBLE_EQ(round(a.y * 1e3) / 1e3f, round(b.y * 1e3) / 1e3f);
		ASSERT_DOUBLE_EQ(round(a.z * 1e3) / 1e3f, round(b.z * 1e3) / 1e3f);
	}
};


TEST_F(TranslatorTests, t1)
{
	auto T = Translator(0, 0, 0, 0);
	auto p = T.Position({ 2, 3, 1 });
	auto v = T.Velocity({ 3, 4, 2 });
	Compare(p, Translator::FVector( 3, 1, 2 ));
	Compare(v, Translator::FVector( 4, 2, 3 ));
}


TEST_F(TranslatorTests, t2)
{
	auto T = Translator(45, 45, 0, 1);
	auto p = T.Position({ 0, 0, 0 });
	auto a = DEG2RAD(45);
	Compare(p, Translator::FVector(
		Math::Cos(a) * Math::Cos(a),
		Math::Cos(a) * Math::Sin(a),
		Math::Sin(a)
	));
}

TEST_F(TranslatorTests, t3)
{
	auto T = Translator(45, 45, 0, 1);
	auto p = T.Position({ 0, 1, 0 });
	auto a = DEG2RAD(45);
	Compare(p, Translator::FVector(
		Math::Cos(a) * Math::Cos(a),
		Math::Cos(a) * Math::Sin(a),
		Math::Sin(a)
	) * 2);
}

TEST_F(TranslatorTests, t4)
{
	auto T = Translator(45, 45, 45, 1);
	auto p = T.Position({ 0, 1, 0 });
	auto a = DEG2RAD(45);
	Compare(p, Translator::FVector(
		Math::Cos(a) * Math::Cos(a),
		Math::Cos(a) * Math::Sin(a),
		Math::Sin(a)
	) * 2);
}

TEST_F(TranslatorTests, t5)
{
	auto T = Translator(90, 0, 0, 1);
	auto p = T.Position({ 1, 2, 3 });
	auto v = T.Velocity({ 2, 3, 4 });
	Compare(p, Translator::FVector(-3, 3, 1));
	Compare(v, Translator::FVector(-4, 3, 2));
}

TEST_F(TranslatorTests, t6)
{
	auto T = Translator(0, 90, 0, 1);
	auto p = T.Position({ 1, 2, 3 });
	auto v = T.Velocity({ 2, 3, 4 });
	Compare(p, Translator::FVector(-1, 3, 3));
	Compare(v, Translator::FVector(-2, 4, 3));
}

TEST_F(TranslatorTests, t7)
{
	auto T = Translator(0, 0, 90, 1);
	auto p = T.Position({ 1, 2, 3 });
	auto v = T.Velocity({ 2, 3, 4 });
	Compare(p, Translator::FVector(3, 1, -3));
	Compare(v, Translator::FVector(3, 2, -4));
}

TEST_F(TranslatorTests, t8)
{
	auto T = Translator(90, 0, 45, 1);
	auto p = T.Position({ 1, 2, 3 });
	auto v = T.Velocity({ 2, 3, 4 });
	auto a = DEG2RAD(45);
	Compare(p, Translator::FVector(-3 * Math::Cos(a) - 1 * Math::Sin(a), 3, 1 * Math::Cos(a) - 3 * Math::Sin(a)));
	Compare(v, Translator::FVector(-4 * Math::Cos(a) - 2 * Math::Sin(a), 3, 2 * Math::Cos(a) - 4 * Math::Sin(a)));
}
