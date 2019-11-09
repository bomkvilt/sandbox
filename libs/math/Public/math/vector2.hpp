#ifndef MATH_VECTOR2_HPP
#define MATH_VECTOR2_HPP

#include <string>
#include "math/math.hpp"
#include "math/math_common.hpp"
#include "reflect/reflect.hpp"


template<typename T>
struct TVector2 : public reflect::FArchived
{
	ARCH_BEGIN(reflect::FArchived)
		ARCH_FIELD(, , x)
		ARCH_FIELD(, , y)
		ARCH_END();
	using SVector2 = TVector2<T>;

public: //--------------| data
	T x;
	T y;

public: //--------------| generation
	TVector2(T v = 0) : x(v), y(v)
	{}

	TVector2(T x, T y) : x(x), y(y)
	{}

	template<typename Q>
	TVector2(const TVector2<Q>& r) : x(r.x), y(r.y)
	{}

	template<typename Q>
	TVector2(const TVector<Q>& r) : x(r.x), y(r.y)
	{}

	template<typename Q>
	SVector2& operator=(const TVector2<Q>& r) { VECTOR2_VECTOR2((*this), r, =, STATE); }
	      T& operator[](Int i)       { switch (i) { case 0: return x; case 1: return y; } throw std::out_of_range("Invalid index"); }
	const T& operator[](Int i) const { switch (i) { case 0: return x; case 1: return y; } throw std::out_of_range("Invalid index"); }
	SVector2 Sign() const { return { VECTOR2_SCALAR((*this), >= 0 ? 1 : -1, , LIST) }; }

public: //--------------| vector - vector
	template<typename Q> SVector2 operator+(const TVector2<Q>& r) const { return { VECTOR2_VECTOR2((*this), r, +, LIST) }; }
	template<typename Q> SVector2 operator-(const TVector2<Q>& r) const { return { VECTOR2_VECTOR2((*this), r, -, LIST) }; }
	template<typename Q> SVector2 operator*(const TVector2<Q>& r) const { return { VECTOR2_VECTOR2((*this), r, *, LIST) }; }
	template<typename Q> SVector2 operator/(const TVector2<Q>& r) const { return { VECTOR2_VECTOR2((*this), r, /, LIST) }; }

	template<typename Q> SVector2& operator+=(const TVector2<Q>& r) { VECTOR2_VECTOR2((*this), r, +=, STATE); return *this; }
	template<typename Q> SVector2& operator-=(const TVector2<Q>& r) { VECTOR2_VECTOR2((*this), r, -=, STATE); return *this; }
	template<typename Q> SVector2& operator*=(const TVector2<Q>& r) { VECTOR2_VECTOR2((*this), r, *=, STATE); return *this; }
	template<typename Q> SVector2& operator/=(const TVector2<Q>& r) { VECTOR2_VECTOR2((*this), r, /=, STATE); return *this; }

public: //--------------| vector - scalar
	SVector2 operator+(const T& r) const { return { VECTOR2_SCALAR((*this), r, +, LIST) }; }
	SVector2 operator-(const T& r) const { return { VECTOR2_SCALAR((*this), r, -, LIST) }; }
	SVector2 operator*(const T& r) const { return { VECTOR2_SCALAR((*this), r, *, LIST) }; }
	SVector2 operator/(const T& r) const { return { VECTOR2_SCALAR((*this), r, /, LIST) }; }

	SVector2& operator+=(const T& r) { VECTOR2_SCALAR((*this), r, +=, STATE); return *this; }
	SVector2& operator-=(const T& r) { VECTOR2_SCALAR((*this), r, -=, STATE); return *this; }
	SVector2& operator*=(const T& r) { VECTOR2_SCALAR((*this), r, *=, STATE); return *this; }
	SVector2& operator/=(const T& r) { VECTOR2_SCALAR((*this), r, /=, STATE); return *this; }

public: //--------------| unary
	SVector2 operator-() const
	{
		return { VECTOR2_SCALAR((*this), -1, *, LIST) };
	}

public: //--------------| comparement
	template<typename Q>
	bool operator==(const TVector2<Q>& r) const
	{
		return VECTOR2_SCALAR((*this), r, == , &);
	}
	template<typename Q>
	bool operator!=(const TVector2<Q>& r) const
	{
		return !((*this) == r);
	}

public: //--------------| size
	T Sum()   const { return VECTOR2_SCALAR((*this), , , +); }
	T Size2() const { return VECTOR2_VECTOR2((*this), (*this), *, +); }
	T Size()  const { return Math::Sqrt(Size2()); }

	SVector2 GetNormal() const
	{
		auto size = Size();
		return { VECTOR2_SCALAR((*this), size, / , LIST) };
	}

	SVector2& Normalize()
	{
		auto size = Size();
		VECTOR2_SCALAR((*this), size, /=, STATE);
		return *this;
	}

public: //--------------| misc

	template<typename Q, typename R>
	SVector2 Lerp(const TVector2<Q> & r, const R & factor) const
	{
		return (*this) - (r - (*this)) * factor;
	}

	std::string ToString() const
	{
		auto ss = std::ostringstream();
		ss 	<< "x: " << x << " "
			<< "y: " << y;
		return ss.str();
	}

	const static TVector2<T> ZeroVector;
};

template<typename T> inline TVector2<T> operator+(T r, const TVector2<T> & v) { return v + r; }
template<typename T> inline TVector2<T> operator-(T r, const TVector2<T> & v) { return v - r; }
template<typename T> inline TVector2<T> operator*(T r, const TVector2<T> & v) { return v * r; }
template<typename T> inline TVector2<T> operator/(T r, const TVector2<T> & v) { return v / r; }

template<typename T>
const TVector2<T> TVector2<T>::ZeroVector(0);


// ----------------| Math::
namespace Math
{
	template<typename T>
	TVector2<T> Sqrt(const TVector2<T>& x)
	{
		return {
			Sqrt(x.x),
			Sqrt(x.y)};
	}

	template<typename T, typename Q>
	TVector2<T> Clamp(const TVector2<T>& v, Q min, Q max)
	{
		return {
			Clamp(v.x, min, max),
			Clamp(v.y, min, max)};
	}
}


#endif //!MATH_VECTOR2_HPP
