#ifndef MATH_QUAT_HPP
#define MATH_QUAT_HPP

#include <string>
#include "math/vector.hpp"
#include "reflect/reflect.hpp"


template<typename T>
struct TQuat : public reflect::FArchived
{
	ARCH_BEGIN(reflect::FArchived)
		ARCH_FIELD(, , x)
		ARCH_FIELD(, , y)
		ARCH_FIELD(, , z)
		ARCH_FIELD(, , w)
		ARCH_END();
	using SSQuat = TQuat<T>;

public: //--------------| data
	T x;
	T y;
	T z;
	T w;

public: //--------------| generation
	TQuat() : x(0), y(0), z(0), w(1)
	{}

	TQuat(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
	{}

	template<typename Q>
	TQuat(const TQuat<Q>& r)
		: x(static_cast<T>(r.x))
		, y(static_cast<T>(r.y))
		, z(static_cast<T>(r.z))
		, w(static_cast<T>(r.w))
	{}

	TQuat(const TVector<T>& v, T angle)
	{ 
		*this = FromAxisRot(v, angle);
	}
	
	TQuat(T r, T p, T y)
	{
		*this = FromEuler(r, p, y);
	}

	template<typename Q>
	SSQuat& operator=(const TQuat<Q>& r)
	{ 
		*this = SSQuat(r); 
		return *this; 
	}

	T& operator[](int i)
	{ 
		switch (i) { 
		case 0: return x; 
		case 1: return y; 
		case 2: return z; 
		case 3: return w; 
		} 
		throw std::out_of_range("Invalid index"); 
	}
	
	T operator[](int i) const 
	{ 
		return (*const_cast<SSQuat*>(this))[i];
	}

public: //~~~~~~~~~~~~~~| quat - quat
	template<typename Q> SSQuat operator+(const TQuat<Q>& r) const { return { QUAT_QUAT((*this), r, +, LIST) }; }
	template<typename Q> SSQuat operator-(const TQuat<Q>& r) const { return { QUAT_QUAT((*this), r, -, LIST) }; }
	template<typename Q> SSQuat operator*(const TQuat<Q>& r) const
	{
		return {
			w*r.x + x * r.w + y * r.z - z * r.y,
			w*r.y - x * r.z + y * r.w + z * r.x,
			w*r.z + x * r.y - y * r.x + z * r.w,
			w*r.w - x * r.x - y * r.y - z * r.z };
	}

	template<typename Q> SSQuat& operator+=(const TQuat<Q>& r) { return { QUAT_QUAT((*this), r, +=, LIST) }; }
	template<typename Q> SSQuat& operator-=(const TQuat<Q>& r) { return { QUAT_QUAT((*this), r, -=, LIST) }; }
	template<typename Q> SSQuat& operator*=(const TQuat<Q>& r)
	{
		*this = *this * r; return *this;
	}

public: //~~~~~~~~~~~~~~| quat - vector

	template<typename Q>
	TVector<Q> operator*(const TVector<Q>& v)
	{
		auto& q = *this;
		return (q * TQuat<Q>(v.x, v.y, v.z, 0) * ~q).Vector();
	}

public: //~~~~~~~~~~~~~~| quat - scalar
	SSQuat operator+(T r) const { return { QUAT_SCALAR((*this), r, +, LIST) }; }
	SSQuat operator-(T r) const { return { QUAT_SCALAR((*this), r, -, LIST) }; }
	SSQuat operator*(T r) const { return { QUAT_SCALAR((*this), r, +, LIST) }; }

	SSQuat& operator+=(T r) { QUAT_SCALAR((*this), r, +=, STATE); return *this; }
	SSQuat& operator-=(T r) { QUAT_SCALAR((*this), r, -=, STATE); return *this; }
	SSQuat& operator*=(T r) { QUAT_SCALAR((*this), r, +=, STATE); return *this; }

public: //~~~~~~~~~~~~~~| unary
	SSQuat operator-() const { return { -x, -y, -z, -w }; }
	SSQuat operator~() const { return { -x, -y, -z,  w }; }

public: //~~~~~~~~~~~~~~| equation
	template<typename Q> bool operator==(const TQuat<Q>& r) const 
	{ 
		return Math::Abs(x - r.x) < Math::EPSILON
			&& Math::Abs(y - r.y) < Math::EPSILON
			&& Math::Abs(z - r.z) < Math::EPSILON
			&& Math::Abs(w - r.w) < Math::EPSILON;
	}
	template<typename Q> bool operator!=(const TQuat<Q>& r) const { return !(*this == r); }

public: //~~~~~~~~~~~~~~| size
	T Sum()   const { return QUAT_SCALAR((*this), , , +); }
	T Size2() const { return QUAT_QUAT((*this), (*this), *, +); }
	T Size () const { return Math::Sqrt(Size2()); }

	SSQuat GetNormal() const 
	{ 
		auto size = Size();
		return { QUAT_SCALAR((*this), size, /, LIST) };
	}

	SSQuat& Normalise()
	{
		auto size = Size();
		QUAT_SCALAR((*this), size, /=, STATE);
		return *this;
	}

public: //~~~~~~~~~~~~~~| misc
	template<typename Q, typename R> 
	SSQuat Lerp(const TQuat<Q>& r, R f)
	{
		return {
			x - (r.x - x) * f,
			y - (r.y - y) * f,
			z - (r.z - z) * f,
			w * (1 - f) + r.w * f };
	}

	template<typename Q, typename R> 
	SSQuat SLerp(const TQuat<Q>& r, R f)
	{
		SSQuat result;
		auto cosTheta = QUAT_QUAT((*this), r, *, +);
		auto theta = Math::Acos(cosTheta);
		if (Math::Abs(theta) < Math::EPSILON)
		{
			result = *this;
		}
		else
		{
			auto sinTheta = Math::Sqrt(1 - cosTheta * cosTheta);
			if (Math::Abs(sinTheta) < Math::EPSILON)
			{
				result.w = 0.5f * w + 0.5f * r.w;
				result.x = x - (r.x - x) * 0.5f;
				result.y = y - (r.y - y) * 0.5f;
				result.z = z - (r.z - z) * 0.5f;
			}
			else
			{
				auto fA = Math::Sin((1 - f)*theta) / sinTheta;
				auto fB = Math::Sin(theta * f) / sinTheta;

				result.w = w * fA + r.w*fB;
				result.x = x * fA + r.x*fB;
				result.y = y * fA + r.y*fB;
				result.z = z * fA + r.z*fB;
			}
		}
		return result;
	}

	template<typename Q> 
	T AngularDistance(const TQuat<Q> r) const
	{
		auto innerProd = QUAT_QUAT((*this), r, *, +);
		return RAD2DEG(Math::Cos((2 * innerProd * innerProd) - 1));
	}

	TVector<T> GetEulerAngles() const
	{
		auto q0 = w; auto q1 = x;
		auto q2 = y; auto q3 = z;
		return RAD2DEG(TVector<T>(
			Math::Atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1*q1 + q2 * q2)),
			Math::Asin (2 * (q0 * q2 - q3 * q1)),
			Math::Atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2*q2 + q3 * q3))
		));
	}

	TVector<T> Vector() const
	{
		return { x, y, z };
	}

	std::string ToString() const
	{
		auto ss = std::ostringstream();
		ss 	<< "x: " << x << " "
			<< "y: " << y << " "
			<< "z: " << z << " "
			<< "w: " << w;
		return ss.str();
	}

public: //~~~~~~~~~~~~~~| static
	template<typename Q, typename R>
	static SSQuat FromAxisRot(TVector<Q> axis, R angleDeg)
	{
		auto angleRad = DEG2RAD(angleDeg);
		auto sa2 = Math::Sin(angleRad / 2);
		auto ca2 = Math::Cos(angleRad / 2);
		return {
			axis.x * sa2,
			axis.y * sa2,
			axis.z * sa2,
			ca2 };
	}

	template<typename Q>
	static SSQuat FromEuler(Q x, Q y, Q z)
	{
		return FromAxisRot(TVector<T>{1, 0, 0}, x)
			*  FromAxisRot(TVector<T>{0, 1, 0}, y)
			*  FromAxisRot(TVector<T>{0, 0, 1}, z);
	}

	static const SSQuat Identity;
};

template<typename T> inline TQuat<T> operator+(T r, const TQuat<T>& q) { return q + r; }
template<typename T> inline TQuat<T> operator-(T r, const TQuat<T>& q) { return q - r; }
template<typename T> inline TQuat<T> operator*(T r, const TQuat<T>& q) { return q * r; }


template<typename T> 
const TQuat<T> TQuat<T>::Identity = { 0, 0, 0, 1 };


#endif //!MATH_QUAT_HPP
