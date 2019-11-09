#ifndef MATH_MATH_HPP
#define MATH_MATH_HPP

#include <algorithm>
#include "Math/math_common.hpp"


namespace Math
{
	// constants
	
	constexpr FReal PI      = static_cast<FReal>(3.141593);
	constexpr FReal EPSILON = static_cast<FReal>(1e-05);

	// misc

	template<typename T, typename Q>
	T Clamp(T v, Q min, Q max)
	{
		return v > max ? max
			: v < min ? min
			: v;
	}
	
	float  Sqrt(float  x);
	double Sqrt(double x);

	float  Cos(float  x);
	double Cos(double x);
	float  Sin(float  x);
	double Sin(double x);
	float  Tan(float  x);
	double Tan(double x);

	float  Acos(float  x);
	double Acos(double x);
	float  Asin(float  x);
	double Asin(double x);
	float  Atan(float  x);
	double Atan(double x);
	float  Atan2(float  x, float  y);
	double Atan2(double x, double y);

	float  Abs(float  x);
	double Abs(double x);
	float  Max(float  x, float  y);
	double Max(double x, double y);
	float  Min(float  x, float  y);
	double Min(double x, double y);
}

#endif //!MATH_MATH_HPP
