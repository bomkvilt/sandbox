#ifndef CODEFORCE_CPP__2100_617F_BUTYOFBERLAND_HPP
#define CODEFORCE_CPP__2100_617F_BUTYOFBERLAND_HPP
#define PROBLEM_NAME ButyOfBerland
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <unordered_map>
#include <functional>
#include <iostream>
#include <vector>
#include <deque>
#include <map>




class PROBLEM_NAME
{
private:
	using LinkKey = std::pair<int, int>;

	struct LinkKeyHasher
	{
		using argument_type = LinkKey;
		using result_type   = size_t;
		size_t operator()(const LinkKey& key) const noexcept {
			return 
				std::hash<int>()(key.first) + 
				std::hash<int>()(key.second) << 32;
		}
	};

	template<typename K, typename D = int>
	class TPathTree
	{
	public:
		using Links  = std::unordered_multimap<K, K>;
		using Depths = std::unordered_map<K, D>;
		using Roots  = std::unordered_map<K, K>;

	public:
		TPathTree(size_t expectedSize = 0)
		{
			depths.reserve(expectedSize);
			roots.reserve(expectedSize);
			links.reserve(expectedSize * 2);
		}

		void AddLink(const K& p, const K& q)
		{
			links.insert({ p, q });
			links.insert({ q, p });
		}

		void ApplyLinks()
		{
			if (links.size() == 0)
			{
				return;
			}

			// compute root node maximal depth
			auto root = links.begin()->first;
			ComputeRootDepth(root);
			if (links.size() > 2)
			{
				// find an optimal root position
				root = FindEquilibriumRoot(root);
			}		
			// compute node depths
			ComputeNodeDepths(root);
		}

		auto FindPath(K a, K b) -> std::vector<K>
		{
			auto patha = std::vector<K>();
			auto pathb = std::vector<K>();

			// place the paths iterators on the same depths
			auto da = depths[a];
			auto db = depths[b];
			for (; da > db; --da, a = roots[a])
			{
				patha.push_back(a);
			}
			for (; db > da; --db, b = roots[b])
			{
				pathb.push_back(b);
			}

			// find first common node
			for (; a != b; a = roots[a], b = roots[b])
			{
				patha.push_back(a);
				pathb.push_back(b);
			}
			patha.push_back(a);

			patha.reserve(patha.size() + pathb.size());
			auto pos = pathb.rbegin();
			auto end = pathb.rend();
			for (; pos != end; ++pos)
			{
				patha.push_back(*pos);
			}
			return patha;
		}

	private:
		void ComputeRootDepth(const K& root)
		{
			ClearTreeStructure();
			auto path = std::vector{ std::pair{ root, root } };
			auto positions = std::unordered_map<K, LinkIterator>();
			while (path.size())
			{
				auto& [node, parent] = path.back();

				auto chldPos = positions.find(node);
				auto chldEnd = positions.end();
				if (chldPos == chldEnd)
				{	// the node have never been visited
					roots[node] = parent;
					depths[node] = 0;
					positions[node] = MakeLinkIterator(node);
				}
			
				if (auto& childIt = positions[node]; !childIt)
				{	// update parent node's depth
					if (node != parent)
					{	
						auto pDep = depths[parent];
						auto nDep = depths[node] + 1;
						auto mDep = pDep > nDep ? pDep : nDep;
						depths[parent] = mDep;
					}
					// all child nodes are processed
					positions.erase(node);
					path.pop_back();
					continue;
				}
				else path.emplace_back((childIt++)->second, node);
			}
		}

		void ComputeNodeDepths(const K& root)
		{
			auto parents  = std::vector<K>{root};
			auto children = std::vector<K>();
			int  curDepth = 0;
			while (parents.size())
			{
				for (auto node : parents)
				{
					for (auto childPos = MakeLinkIterator(node); childPos; ++childPos)
					{
						auto child = childPos->second;
						children.push_back(child);
					}
					depths[node] = curDepth;
				}
				std::swap(parents, children);
				children.clear();
				++curDepth;
			}
		}

		K FindEquilibriumRoot(K root)
		{
			auto setNewRoot = [&](int newDepth, const K& newRoot)
			{
				depths[root] = newDepth;
				roots[root] = newRoot;
				root = newRoot;
				roots[root] = root;
			};

			auto children = std::multimap<int, K, std::greater<int>>();
			while (true)
			{
				children.clear();			
				for (auto childPos = MakeLinkIterator(root); childPos; ++childPos)
				{
					auto child = childPos->second;
					children.insert({ depths[child], child });
				}

				auto sortedPos = children.begin();
				auto [max_1, node_1] = *sortedPos;
				if (children.size() == 1)
				{
					setNewRoot(0, node_1);
					continue;
				}

				auto [max_2, node_2] = *(++sortedPos);
				if (max_1 - max_2 > 1)
				{ 
					setNewRoot(max_2 + 1, node_1);
				}
				else break;
			}
			return root;
		}
	
		void ClearTreeStructure()
		{
			depths.clear();
			roots .clear();
			depths.reserve(links.size() / 2);
			roots .reserve(links.size() / 2);
		}

	private:
		struct LinkIterator
		{
		public:
			using Iterator = typename Links::iterator;
			K key = 0; K root = 0;
			Iterator pos;
			Iterator end;

		public:
			LinkIterator() = default;
			LinkIterator(K key, K root, Iterator pos, Iterator end)
				: key(key), root(root)
				, pos(pos), end(end)
			{
				if (pos->second == root)
				{ 
					++(*this); 
				}
			}

			LinkIterator& operator++()
			{
				if (*this) 
				{ 
					++pos; 
				}
				while (*this && pos->second == root)
				{ 
					++pos; 
				}
				return *this;
			}

			LinkIterator operator++(int)
			{
				auto tmp = *this;
				return ++(*this), tmp;
			}

			operator bool() const
			{ return pos != end && pos->first == key; }

			auto operator->()
			{ return &*pos; }
		};

		LinkIterator MakeLinkIterator(const K& node, bool bExcludeParent = true)
		{ 
			return { node, bExcludeParent ? roots[node] : node, links.find(node), links.end()}; 
		}

	private:
		Depths depths;
		Links links;
		Roots roots;
	};

public:
	ButyOfBerland()
		: map(5500)
	{
		sequence.reserve(5500);
		votes.reserve(5500);
		buty.reserve(5500);
	}

	void AddLink(int p, int q)
	{
		map.AddLink(p, q);
		auto key = MakeLinkKey(p, q);
		sequence.push_back(key);
	}

	void ApplyMap()
	{
		map.ApplyLinks();
	}

	bool AddVote(int a, int b, int minButy)
	{
		auto path = map.FindPath(a, b);
		for (int i = 0, j = 1; j < path.size(); ++i, ++j)
		{
			auto p = path[i];
			auto q = path[j];
			auto& curState = buty[MakeLinkKey(p, q)];
			if (curState < minButy)
			{
				curState = minButy;
			}
		}
		votes.emplace_back(std::move(path), minButy);
		maxButy = maxButy > minButy ? maxButy : minButy;
		return true;
	}

	bool CheckConsistency() const
	{
		for (auto& [path, vote] : votes)
		{
			auto minValue = 1000000;
			for (int i = 0, j = 1; j < path.size(); ++i, ++j)
			{
				auto p = path[i];
				auto q = path[j];
				auto& curState = buty.find(MakeLinkKey(p, q))->second;
				if (curState < minValue)
				{
					minValue = curState;
				}
			}
			if (minValue != vote)
			{
				return false;
			}
		}
		return true;
	}

	auto GetButy() -> std::vector<int>
	{
		auto butyList = std::vector<int>();
		butyList.reserve(sequence.size());
		for (auto key : sequence)
		{
			auto value = buty[key];
			butyList.push_back(value != 0 ? value : maxButy);
		}
		return butyList;
	}

private:
	auto MakeLinkKey(int p, int q) const -> LinkKey
	{
		return {
			p < q ? p : q,
			p > q ? p : q
		};
	}

private:
	std::unordered_map<LinkKey, int, LinkKeyHasher> buty;
	std::vector<LinkKey> sequence;
	TPathTree<int> map;
	int maxButy = 0;

	std::vector<std::pair<std::vector<int>, int>> votes;
};


PROBLEM_MAIN()
{
	int n = 0;
	std::cin >> n;

	auto solver = ButyOfBerland();
	while (--n)
	{
		auto p = 0, q = 0;
		std::cin >> p >> q;
		solver.AddLink(p, q);
	}
	solver.ApplyMap();

	int m = 0;
	std::cin >> m;
	while (m--)
	{
		auto a = 0, b = 0, vote = 0;
		std::cin >> a >> b >> vote;
		solver.AddVote(a, b, vote);
	}

	if (!solver.CheckConsistency())
	{
		std::cout << -1 << std::endl;
		return 0;
	}

	auto map = solver.GetButy();
	for (auto v : map)
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__2100_617F_BUTYOFBERLAND_HPP
