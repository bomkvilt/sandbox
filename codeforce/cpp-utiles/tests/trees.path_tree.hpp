#ifndef CODEFORCE_CPP_UTILES__TREES_PATH_TREE_HPP
#define CODEFORCE_CPP_UTILES__TREES_PATH_TREE_HPP

#include <unordered_map>
#include <functional>
#include <vector>
#include <map>



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


#endif //!CODEFORCE_CPP_UTILES__TREES_PATH_TREE_HPP
