#ifndef CODEFORCE_CPP__2500_GLOB8F_SKIACCIDENTS_HPP
#define CODEFORCE_CPP__2500_GLOB8F_SKIACCIDENTS_HPP
#define PROBLEM_NAME SkiAccidents
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <unordered_map>
#include <functional>
#include <iostream>
#include <vector>
#include <array>



class PROBLEM_NAME
{
private:
	using LinkTable = std::unordered_map<int, std::array<int, 2>>;
	using Paths     = std::vector<std::array<int, 3>>;

public:
	PROBLEM_NAME() = default;

	void AddLink(int node, int child)
	{
		links.emplace(node, child);
	}

	auto MakeSimple() -> std::vector<int>
	{
		auto deleted = std::vector<int>();
		auto score = std::unordered_map<int, int>();
		auto paths = FindLongPaths(); // O(n)

		for (; paths.size(); score.clear()) // O(n^2)
		{
			for (auto& path : paths) // O(n)
			{
				for (auto node : path)
				{
					++score[node];
				}
			}
		
			auto maxScore = 0;
			auto maxScoreID = 0;
			for (auto& [node, count] : score) // O(n)
			{
				if (maxScore < count)
				{
					maxScore = count;
					maxScoreID = node;
				}
			}
			RemoveRelatedPaths(paths, maxScoreID); // O(n)
			deleted.push_back(maxScoreID);
		}
		return deleted;
	}

private:
	Paths FindLongPaths()
	{
		auto paths = Paths();
		for (auto& [node, children] : links) // O(n)
		{
			for (auto child : children) // O(1)
			{
				if (!child)
				{
					continue;
				}
				for (auto& subchild : links[child]) // O(1)
				{
					if (!subchild)
					{
						continue;
					}
					paths.emplace_back(node, child, subchild);
				}
			}
		}
		return paths;
	}

	void RemoveRelatedPaths(Paths& paths, int node)
	{
		auto newPaths = Paths();
		newPaths.reserve(paths.size());
		for (auto& path : paths)
		{
			auto bContains = false;
			for (auto& pathNode : path)
			{
				if (pathNode == node)
				{
					bContains = true;
					break;
				}
			}
			if (!bContains)
			{
				newPaths.emplace_back(path);
			}
		}
		std::swap(paths, newPaths);
	}

private:
	LinkTable links;
};



PROBLEM_MAIN()
{

	return 0;
}


#endif //!CODEFORCE_CPP__2500_GLOB8F_SKIACCIDENTS_HPP
