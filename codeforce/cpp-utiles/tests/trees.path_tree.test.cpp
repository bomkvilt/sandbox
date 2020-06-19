#include <gtest/gtest.h>
#include "trees.path_tree.hpp"



TEST(tests, TPathTree_path)
{
	auto scheme = std::vector<std::pair<int, int>>{
		{ 1,  5}, { 2,  5}, { 5,  6}, { 3,  6}, { 6, 18}, { 4, 18}, {18, 26},
		{11, 15}, {15, 13}, {13, 12}, {12, 16}, {16, 14}, {14, 17},
		{ 7, 25}, { 9, 25}, { 8, 27}, {10, 27}, {25, 22}, {27, 22},
		{19, 22}, {23, 22}, {24, 22}, {22, 20}, {21, 20}, {20, 17}, {17, 26}
	};
	auto cases = std::vector<std::tuple<int, int, int, std::vector<int>>>{
		  // case_number, from, to, {path}
		  {1,  1,  3, {1 , 5 , 6 ,  3}}
		, {2, 14,  4, {14, 17, 26, 18, 4}}
		, {3,  7, 12, { 7, 25, 22, 20, 17, 14, 16, 12}}
	};

	auto tree = TPathTree<int>();
	for (auto [p, q] : scheme)
	{
		tree.AddLink(p, q);
	}
	tree.ApplyLinks();

	for (auto [n, a, b, exp] : cases)
	{
		auto res = tree.FindPath(a, b);
		EXPECT_EQ(res, exp) << "n = " << n;
	}
}
