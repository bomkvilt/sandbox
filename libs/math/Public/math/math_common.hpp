#ifndef MATH_MATH_COMMON_HPP
#define MATH_MATH_COMMON_HPP

#include <cinttypes>

//-------| common floating point type

#ifndef bDOUBLE
using FReal = float;
#elif F_REAL == 2
using FReal = double;
#endif //!MATH_MATH_COMMON_HPP
//-------| unsigned int types

using Int8  = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;
using Int   = Int32;

//-------| signed int types

using UInt8  = uint8_t;
using UInt16 = uint16_t;
using UInt32 = uint32_t;
using UInt64 = uint64_t;
using UInt   = UInt32;


//-------| macro

#define DEG2RAD(x) ((x * Math::PI ) / 180)
#define RAD2DEG(x) ((x * 180) / Math::PI )

#define LIST	, /**/
#define STATE	; /**/

#define VECTOR_VECTOR(A, B, OP, SP)		\
	(A.x OP B.x) SP						\
	(A.y OP B.y) SP						\
	(A.z OP B.z) 						\
/**/

#define VECTOR_SCALAR(V, S, OP, SP)		\
	(V.x OP S) SP						\
	(V.y OP S) SP						\
	(V.z OP S) 							\
/**/

#define VECTOR2_VECTOR2(A, B, OP, SP)	\
	(A.x OP B.x) SP						\
	(A.y OP B.y)						\
/**/

#define VECTOR2_SCALAR(V, S, OP, SP)	\
	(V.x OP S) SP						\
	(V.y OP S) 							\
/**/

#define QUAT_QUAT(A, B, OP, SP)			\
	(A.w OP B.w) SP						\
	(A.x OP B.x) SP						\
	(A.y OP B.y) SP						\
	(A.z OP B.z) 						\
/**/

#define QUAT_SCALAR(V, S, OP, SP)		\
	(V.x OP S) SP						\
	(V.y OP S) SP						\
	(V.z OP S) 							\
/**/

#endif //!MATH_MATH_COMMON_HPP
