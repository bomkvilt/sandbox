#ifndef CODEFORCE_CPP_UTILES__MATH_MPOW_HPP
#define CODEFORCE_CPP_UTILES__MATH_MPOW_HPP

#include <stdint.h>



int64_t MPow(int64_t x, int64_t y, int64_t m)
{
	if (x %= m; !x || m == 0)
	{
		return 0;
	}
	if (y == 0)
	{
		return 1;
	}

	int64_t res = 1;
	for (; y > 0; y /= 2)
	{
		if (y & 1)
		{
			res = (res * x) % m;
		}
		x = (x * x) % m;
	}
	return res;
}


#endif //!CODEFORCE_CPP_UTILES__MATH_MPOW_HPP
