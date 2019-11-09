#include "math/color.hpp"


#define OPERATION(_SIGN, _R) \
	R  _SIGN  _R.R; \
	G  _SIGN  _R.G; \
	B  _SIGN  _R.B; \
	A  _SIGN  _R.A;

#define OPERATION_LIST(_SIGN, _R) \
	R  _SIGN  _R.R, \
	G  _SIGN  _R.G, \
	B  _SIGN  _R.B, \
	A  _SIGN  _R.A

#define OPERATION_FLOAT(_SIGN, _R) \
	R  _SIGN  _R; \
	G  _SIGN  _R; \
	B  _SIGN  _R; \
	A  _SIGN  _R;

#define OPERATION_LIST_FLOAT(_SIGN, _R) \
	R  _SIGN  _R, \
	G  _SIGN  _R, \
	B  _SIGN  _R, \
	A  _SIGN  _R


FColor::FColor(FReal R, FReal G, FReal B, FReal A)
	: R(R), G(G), B(B), A(A)
{}

FColor::FColor(const FColor& r)
	: R(r.R), G(r.G), B(r.B), A(r.A)
{}

FColor& FColor::operator=(const FColor& r)
{
	OPERATION(=, r);
	return *this;
}

FReal& FColor::operator[](int i)
{
	switch (i) {
	case 0: return R;
	case 1: return G;
	case 2: return B;
	case 3: return A;
	}
	throw std::out_of_range("");
}

const FReal& FColor::operator[](int i) const
{
	return const_cast<FColor&>(*this)[i];
}

// FColor

FColor  FColor::operator+(const FColor& r) const { return FColor(OPERATION_LIST(+, r)); }
FColor  FColor::operator-(const FColor& r) const { return FColor(OPERATION_LIST(-, r)); }

FColor& FColor::operator-=(FColor r) { OPERATION(-=, r) return *this; }
FColor& FColor::operator+=(FColor r) { OPERATION(+=, r) return *this; }

// Scalar

FColor  FColor::operator+(FReal r) const { return FColor(OPERATION_LIST_FLOAT(+, r)); }
FColor  FColor::operator-(FReal r) const { return FColor(OPERATION_LIST_FLOAT(-, r)); }
FColor  FColor::operator*(FReal r) const { return FColor(OPERATION_LIST_FLOAT(*, r)); }
FColor  FColor::operator/(FReal r) const { assert(r); return FColor(OPERATION_LIST_FLOAT(/ , r)); }

FColor& FColor::operator+=(FReal r) { OPERATION_FLOAT(+=, r) return *this; }
FColor& FColor::operator-=(FReal r) { OPERATION_FLOAT(-=, r) return *this; }
FColor& FColor::operator*=(FReal r) { OPERATION_FLOAT(*=, r) return *this; }
FColor& FColor::operator/=(FReal r) { assert(r); OPERATION_FLOAT(/=, r) return *this; }

// Comapre

bool FColor::operator==(const FColor& r) const
{
	return R == r.R
		&& G == r.G
		&& B == r.B
		&& A == r.A;
}

bool FColor::operator!=(const FColor& r) const
{
	return !(*this == r);
}

// Misc

FColor FColor::Lerp(const FColor & r, FReal factor)
{
	return (*this) - (r - (*this)) * factor;
}

std::string FColor::ToString() const
{
	auto ss = std::ostringstream();
	ss	<< "R: " << R << " "
		<< "G: " << G << " "
		<< "B: " << B << " "
		<< "A: " << A;
	return ss.str();
}

FColor operator+(FReal r, const FColor& c) { return c + r; }
FColor operator-(FReal r, const FColor& c) { return c - r; }
FColor operator*(FReal r, const FColor& c) { return c * r; }
FColor operator/(FReal r, const FColor& c) { return c / r; }
