#ifndef MATH_COLOR_HPP
#define MATH_COLOR_HPP

#include "math/math.hpp"
#include "math/math_common.hpp"
#include "reflect/reflect.hpp"


struct FColor : public reflect::FArchived
{
	ARCH_BEGIN(reflect::FArchived)
		ARCH_FIELD(, , R)
		ARCH_FIELD(, , G)
		ARCH_FIELD(, , B)
		ARCH_FIELD(, , A)
		ARCH_END();
public:

	FReal R;
	FReal G;
	FReal B;
	FReal A;

public:

	FColor(FReal R = 0, FReal G = 0, FReal B = 0, FReal A = 0);
	FColor(const FColor& r);

	FColor& operator=(const FColor& r);

	FReal& operator[](int i);
	const FReal& operator[](int i) const;

public: //~~~~~~~~~~~~~~| color - color

	FColor operator+(const FColor& r) const;
	FColor operator-(const FColor& r) const;

	FColor& operator-=(FColor r);
	FColor& operator+=(FColor r);

public: //~~~~~~~~~~~~~~| color - scalar

	FColor operator+(FReal r) const;
	FColor operator-(FReal r) const;
	FColor operator*(FReal r) const;
	FColor operator/(FReal r) const;

	// ?=

	FColor& operator+=(FReal r);
	FColor& operator-=(FReal r);
	FColor& operator*=(FReal r);
	FColor& operator/=(FReal r);

public: //~~~~~~~~~~~~~~| equation

	bool operator==(const FColor& r) const;
	bool operator!=(const FColor& r) const;

public: //~~~~~~~~~~~~~~| misc

	FColor Lerp(const FColor& r, FReal factor);

	std::string ToString() const;
};

FColor operator+(FReal r, const FColor& c);
FColor operator-(FReal r, const FColor& c);
FColor operator*(FReal r, const FColor& c);
FColor operator/(FReal r, const FColor& c);


#endif //!MATH_COLOR_HPP
