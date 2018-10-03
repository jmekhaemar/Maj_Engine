#ifndef _VEC4_H_
#define _VEC4_H_

#include <iostream>
#include <string>
#include <ISerializable.h>

namespace maj::math
{
	template<typename T>
	struct Vec4Holder final
	{
		// Returns X if index is out of range
		T operator[](uint8_t index) const;
		// Returns X if index is out of range
		T& operator[](uint8_t index);

		bool operator==(const Vec4Holder& kRhs) const;
		bool operator!=(const Vec4Holder& kRhs) const;

		T mX;
		T mY;
		T mZ;
		T mW;
	};

	template <typename T>
	class Vec4 : public ISerializable
	{
		ENABLE_META(Vec4<T>)

	public:
		// Percent will be clamped to [0, 1]
		static Vec4 lerp(const Vec4& kStart, const Vec4& kEnd, T percent);
		// Percent will be clamped to [0, 1]
		static Vec4 slerp(Vec4 kStart, Vec4 kEnd, T percent);

		static const Vec4 skRight;
		static const Vec4 skUp;
		static const Vec4 skForward;

	public:
		LOAD_CONSTRUCTOR(Vec4<T>)

		Vec4(T val = static_cast<T>(0));
		Vec4(T x, T y, T z, T w);
		Vec4(const Vec4Holder<T>& kOther);
		Vec4(const Vec4& kOther);
		~Vec4() = default;

		T norm() const;
		T sqrNorm() const;

		// Returns identity if norm is null
		Vec4 normalized() const;
		// Returns identity if Vec4.xyzw is null 
		Vec4 inversed() const;
		// Returns identity if any of Vec4.w is null 
		Vec4 homogenized() const;
		Vec4 scaled(const Vec4& kRhs) const;

		// Does nothing if norm is null
		void normalize();
		// Does nothing if Vec4.w is null 
		void homogenize();
		// Does nothing if any of Vec4.xyzw is null 
		void inverse();

		// Returns an angle in degree
		T angle(const Vec4& kOther) const;
		T dot(const Vec4& kOther) const;
		Vec4 clamp(const Vec4& kMin, const Vec4& kMax) const;
		
		Vec4Holder<T> data() const;

		Vec4 operator*(T rhs) const;
		// Returns identity if the value is null
		Vec4 operator/(T rhs) const;
		Vec4 operator+(const Vec4& kRhs) const;
		Vec4 operator-(const Vec4& kRhs) const;

		Vec4 operator-() const;
		Vec4& operator=(const Vec4& kRhs);
		Vec4& operator*=(T rhs);
		// Does noting if the value is null
		Vec4& operator/=(T rhs);
		Vec4& operator+=(const Vec4& kRhs);
		Vec4& operator-=(const Vec4& kRhs);

		// Returns X if index is out of range
		T operator[](uint8_t index) const;
		// Returns X if index is out of range
		T& operator[](uint8_t index);

		bool operator==(const Vec4& kRhs) const;
		bool operator!=(const Vec4& kRhs) const;

		T mX;
		T mY;
		T mZ;
		T mW;
	};

	// Returns identity if any of Vec4.xyzw is null
	template<typename T>
	Vec4<T> operator/(T lhs, const Vec4<T>& kRhs);

	template<typename T>
	std::ostream& operator<< (std::ostream& os, const Vec4<T>& kRhs);

	typedef Vec4<float> Vec4f;
	typedef Vec4<uint8_t> Vec4ui8;
}

META_CLASS(maj::math::Vec4f, 4u)
META_FIELD(maj::math::Vec4f, float, mX, 0u)
META_FIELD(maj::math::Vec4f, float, mY, 1u)
META_FIELD(maj::math::Vec4f, float, mZ, 2u)
META_FIELD(maj::math::Vec4f, float, mW, 3u)

META_CLASS(maj::math::Vec4ui8, 4u)
META_FIELD(maj::math::Vec4ui8, uint8_t, mX, 0u)
META_FIELD(maj::math::Vec4ui8, uint8_t, mY, 1u)
META_FIELD(maj::math::Vec4ui8, uint8_t, mZ, 2u)
META_FIELD(maj::math::Vec4ui8, uint8_t, mW, 3u)
namespace std
{
	template<typename T>
	struct hash<maj::math::Vec4Holder<T>>
	{
		size_t operator()(const maj::math::Vec4Holder<T>& kVec) const;
	};

	template<typename T>
	struct hash<maj::math::Vec4<T>>
	{
		size_t operator()(const maj::math::Vec4<T>& kVec) const;
	};
}

#include "Vec4.inl"

#endif