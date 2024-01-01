#ifndef CODEFORCE_CPP__800_HELLO2019A_GENADIYANDAKARDGAME_HPP
#define CODEFORCE_CPP__800_HELLO2019A_GENADIYANDAKARDGAME_HPP
#define PROBLEM_NAME GenadiyAndAKardGame
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <array>



PROBLEM_MAIN()
{
	std::string c0;
	std::cin >> c0;

	bool ok = false;
	std::string cn;
	for (int i = 0; i < 5; ++i)
	{
		std::cin >> cn;
		if (c0[0] == cn[0] || c0[1] == cn[1])
		{
			std::cout << "YES" << std::endl;
			return 0;
		}
	}
	std::cout << "NO" << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__800_HELLO2019A_GENADIYANDAKARDGAME_HPP
