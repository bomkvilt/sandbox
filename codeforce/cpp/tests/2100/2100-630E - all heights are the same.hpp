#ifndef CODEFORCE_CPP__2100_630E_ALLHEIGHTSARETHESAME_HPP
#define CODEFORCE_CPP__2100_630E_ALLHEIGHTSARETHESAME_HPP
#define PROBLEM_NAME AllHeightsAreTheSame
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>



/** Hints: (feld have n x m size. Initial heights are in range [L, R]. p = R - L)
 *  - absolute values in cells don't metter
 *  > we neet to know if the cell is odd or even
 *
 *  - we have to operatins:
 *  > increase in value of a cell by 2              - doesn't affect it's partity
 *  > increase in value of two adjacent calls by 1  - changes the partity of the cells together
 *  
 *  - to get the game compleatted all cells must be equal
 *  > all cells must have the same partity
 *  > total count of blocks N = n * m * k can be as odd as even
 *
 *  - we have to possible strategies:
 *  > make field odd    - number of initial even elements must be even
 *  > make field even   - number of initial odd  elements nust be even
 *
 *  - hence, if intial feld:
 *  > has odd elements:  
 *      > we have a even solution if we have even number of odd  cells
 *      > we have a odd  solution if we have even number of even cells
 *  > has even elemens:
 *      > we can have both even and odd solutions if we have even number of each group of cells
 *  
 *  - hence, we have the count of initial fields for each class of fields:
 *  > odd  element count: M = p ^ (n * m) - all possible initial fields goes to a win
 *  > even element count: 
 *      > number of even cells must be even
 *      > lets 2*i is a number of even cells
 *		> we have Combination(2*i, m*m) ways to get 2*i even fields
 *		> even fields have E even variants
 *		> odd  fields have O odd  variants (E + O = p)
 *		> hence, M = Sum(i, 0, mn/2, E^(2*i) * O^(n*m - 2*i) * Combination(2*i, m*m))
 *		> the formula is simular to a binomial sequence
 *		> hence, M = ((E + O)^(n*m) + (E - O)^(n*m)) / 2
 */
int64_t TESTNAME(int64_t n, int64_t m, int64_t L, int64_t R)
{
	constexpr auto powm = [](int64_t x, int64_t y, int64_t m)->int64_t
	{
		int64_t res = 1;
		for (; y > 0; y >>= 1)
		{
			if (y & 1)
			{
				res = (res * x) % m;
			}
			x = (x * x) % m;
		}
		return res;
	};

	int64_t b = 998244353;
	int64_t p = (R - L) + 1;
	if ((m * n) % 2)
	{
		return powm(p, n * m, b);
	}
	else
	{
		auto O = p / 2;
		auto E = p - O;
		return (powm(E + O, n * m, 2 * b) + powm(E - O, n * m, 2 * b)) / 2 % b;
	}
}

PROBLEM_MAIN()
{
	auto n = 0, m = 0, L = 0, R = 0;
	std::cin  >> n >> m >> L >> R;
	std::cout << TESTNAME(n, m, L, R) << std::endl;
	return 0;
}


#endif //!CODEFORCE_CPP__2100_630E_ALLHEIGHTSARETHESAME_HPP
