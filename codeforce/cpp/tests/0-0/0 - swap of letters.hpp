#ifndef CODEFORCE_CPP_0_SWAPOFLETTERS_HPP
#define CODEFORCE_CPP_0_SWAPOFLETTERS_HPP

#include <vector>
#include <string>



class SwapOfLetters
{
public:
	using Swaps = std::vector<std::tuple<int, int>>;

	static auto Swap(const std::string& s, const std::string& t) -> std::tuple<Swaps, bool>
	{
		std::vector<int> ab;
		std::vector<int> ba;
		for (int i = 0; i < s.length(); ++i)
		{
			if (s[i] == t[i])
			{
				continue;
			}
			switch (s[i]) {
			case 'a': ab.push_back(i); break;
			case 'b': ba.push_back(i); break;
			}
		}

		if (ab.size() % 2 != ba.size() % 2)
		{
			return { Swaps(), false };
		}

		Swaps swaps;
		for (int i = 0; i + 1 < ba.size(); i += 2) {
			swaps.emplace_back(ab[i], ab[i + 1]);
		}
		for (int i = 0; i + 1 < ab.size(); i += 2) {
			swaps.emplace_back(ba[i], ba[i + 1]);
		}
		if (ab.size() % 2) {
			int x = ab.back();
			int y = ba.back();
			swaps.emplace_back(x, x);
			swaps.emplace_back(x, y);
		}
		return { swaps, true };
	}
};


#endif //!CODEFORCE_CPP_0_SWAPOFLETTERS_HPP
