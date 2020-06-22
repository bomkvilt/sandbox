#ifndef CODEFORCE_CPP__1000_585A_YELLOWCARDS_HPP
#define CODEFORCE_CPP__1000_585A_YELLOWCARDS_HPP
#define PROBLEM_NAME YellowCards
#ifndef PROBLEM_MAIN
#define PROBLEM_MAIN int main
#endif

#include <iostream>
#include <tuple>



class PROBLEM_NAME
{
public:
	PROBLEM_NAME(int a0, int a1, int k0, int k1, int n)
		: a{ a0, a1 }
		, k{ k0, k1 }
		, n(n)
	{}

	std::tuple<int, int> GetMinMax()
	{
		// get min
		int N_min = 0;
		if (auto n1 = a[0]*(k[0]-1) + a[1]*(k[1]-1); n > n1)
		{
			N_min = n - n1;
		}

		// get max
		int N_max = 0;
		int i_max = k[0] > k[1] ? 0 : 1;
		int i_min = !i_max;
		if (int N1 = n / k[i_min]; N1 <= a[i_min])
		{
			N_max = N1;
		}
		else
		{
			int N2 = (n - k[i_min]*a[i_min]) / k[i_max];
			N_max = a[i_min] + N2;
		}

		return { N_min, N_max };
	}

private:
	int a[2] = { 0, 0 };
	int k[2] = { 0, 0 };
	int n = 0;
};



PROBLEM_MAIN()
{
	int a1 = 0;
	int a2 = 0;
	int k1 = 0;
	int k2 = 0;
	int n = 0;
	std::cin
		>> a1 >> a2
		>> k1 >> k2
		>> n;

	auto t = PROBLEM_NAME(a1, a2, k1, k2, n);
	auto [min, max] = t.GetMinMax();

	std::cout << min << " " << max << "\n";
	return 0;
}


#endif //!CODEFORCE_CPP__1000_585A_YELLOWCARDS_HPP
