#ifndef CROSS_BASIS_INTERNAL_TRANSLATOR_HPP
#define CROSS_BASIS_INTERNAL_TRANSLATOR_HPP

#include "math/vector.hpp"
#include "math/quat.hpp"


class Translator
{
public:
	using FQuat   = TQuat<float>;
	using FVector = TVector<float>;

public:
	Translator(float l, float f, float a, float r)
	{
		auto q1 = FQuat(f, 0, 0);
		auto q2 = FQuat(0, 0, l - 90);
		auto q12 = q2 * q1;
		
		auto e0 = q12 * FVector(0, 1, 0);
		r0 = e0 * r;

		auto q3 = FQuat(e0, -90 - a);
		q123 = q3 * q12;
	}

	FVector Position(const FVector& p)
	{
		return q123 * p + r0;
	}

	FVector Velocity(const FVector& v)
	{
		return q123 * v;
	}

private:
	FQuat q123;
	FVector r0;
};


#endif //!CROSS_BASIS_INTERNAL_TRANSLATOR_HPP
