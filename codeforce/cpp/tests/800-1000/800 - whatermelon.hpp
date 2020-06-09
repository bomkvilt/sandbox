#ifndef CODEFORCE_CPP__800_WHATERMELON_HPP
#define CODEFORCE_CPP__800_WHATERMELON_HPP

#include <iostream>


struct Whatermelon
{
	static bool CanBeSplit(int w)
	{
		return w > 2 && !(w % 2);
	}
};


#ifndef TMAIN
#define TMAIN int main
#else
#define TMAINN TMAINJ(Whatermelon)
#endif



TMAIN()
{
	int w = 0;
	std::cin >> w;
	std::cout << (Whatermelon::CanBeSplit(w) ? "YES" : "NO") << std::endl;
	return 0;
}



#endif //!CODEFORCE_CPP__800_WHATERMELON_HPP
