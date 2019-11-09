#ifndef MATH_TRANSFORM_HPP
#define MATH_TRANSFORM_HPP

#include "math/quat.hpp"
#include "math/vector.hpp"


template<typename T>
class TTransform : public reflect::FArchived
{
	ARCH_BEGIN(reflect::FArchived)
		ARCH_FIELD(, , position)
		ARCH_FIELD(, , rotation)
		ARCH_END();
	using STransform = TTransform<T>;
	using SVector = TVector<T>;
	using SQuat = TQuat<T>;

public:
	SVector position; // positions of a center
	SQuat   rotation; // ratation around the center

public:
	TTransform()
		: position(SVector::ZeroVector)
		, rotation(SQuat::Identity)
	{}

	template<typename Q, typename R>
	TTransform(const TQuat<Q>& q, const TVector<R>& v)
		: rotation(q)
		, position(v)
	{}

	template<typename Q>
	TTransform(const TQuat<Q>& q)
		: position(SVector::ZeroVector)
		, rotation(q)
	{}

	template<typename Q>
	TTransform(const TVector<Q>& v)
		: rotation(SQuat::Identity)
		, position(v)
	{}

	template<typename Q>
	TTransform(const TTransform<Q>& r)
		: position(r.position)
		, rotation(r.rotation)
	{}

	template<typename Q>
	TTransform& operator=(const TTransform<Q>& r)
	{
		position = r.position;
		rotation = r.rotation;
		return *this;
	}

public: //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Translations
	/** Apply the transform to the vector
	* @param v - vector in a transform's parent coorditates
	* @return rotated and translated vector					*/
	template<typename Q> TVector<Q> operator()(const TVector<Q>& v) const { return rotation * v + position; }
	template<typename Q> TVector<Q> operator* (const TVector<Q>& v) const { return (*this)(v); }

	/** Apply the transform to the quaternion
	* @param q - quaternion in a transform's parent coorditates
	* @return	- rotated quaternion							*/
	template<typename Q> TQuat<Q> operator()(const TQuat<Q>& q) const { return rotation * q; }
	template<typename Q> TQuat<Q> operator* (const TQuat<Q>& q) const { return (*this)(q); }

	/** Apply the transform to the quaternion
	* @param r - transform in a transform's parent coorditates
	* @return	- rotated quaternion							*/
	template<typename Q> TTransform<Q> operator()(const TTransform<Q>& r) const { return { rotation * r.rotation, (*this) * r.position }; }
	template<typename Q> TTransform<Q> operator* (const TTransform<Q>& r) const { return (*this)(r); };

public: //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| 
	template<typename Q>
	STransform operator+(const TTransform<Q>& r) const
	{
		return {
			rotation * r.rotation,
			position + r.position };
	}

	template<typename Q>
	STransform operator+(const TQuat<Q>& r) const
	{
		return { rotation * r, position };
	}

	template<typename Q>
	STransform operator+(const TVector<Q>& r) const
	{
		return { rotation, position + r };
	}

	template<typename Q> STransform& operator*=(const TTransform<Q>& r) { return *this = r * (*this); }
	template<typename Q> STransform& operator+=(const TTransform<Q>& r) { return *this = (*this) + r; }
	template<typename Q> STransform& operator+=(const TQuat<Q>& r) { return *this = (*this) + r; }
	template<typename Q> STransform& operator+=(const TVector<Q>& r) { return *this = (*this) + r; }

public: //~~~~~~~~~~~~~~| unary

	STransform operator~() const
	{
		auto inv = ~rotation;
		return { inv, inv * -position };
	}

public: //~~~~~~~~~~~~~~| misc

	std::string ToString() const
	{
		return "q:{" + rotation.ToString() + "}"
			+  "p:{" + position.ToString() + "}";
	}

public: //~~~~~~~~~~~~~~| constants

	static const STransform Identity;
};

template<typename T, typename Q>
TTransform<T> operator*(const TQuat<Q>& q, const TTransform<T>& t)
{
	return { q * t.rotation, q * t.position };
}

template<typename T>
const TTransform<T> TTransform<T>::Identity;

#endif //!MATH_TRANSFORM_HPP
