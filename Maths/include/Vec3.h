#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>
#include <string>
#include <ISerializable.h>

namespace maj::math
{
	template<typename T>
	struct Vec3Holder final
	{
		// Returns X if index is out of range
		T operator[](uint8_t index) const;
		// Returns X if index is out of range
		T& operator[](uint8_t index);

		bool operator==(const Vec3Holder& kOther) const;
		bool operator!=(const Vec3Holder& kOther) const;

		T mX;
		T mY;
		T mZ;
	};

	template <typename T>
	class Vec3 final : public ISerializable
	{
		ENABLE_META(Vec3<T>)

	public:
		// Percent will be clamped to [0, 1]
		static Vec3 lerp(const Vec3& kStart, const Vec3& kEnd, T percent);
		// Percent will be clamped to [0, 1]
		static Vec3 slerp(Vec3 kStart, Vec3 kEnd, T percent);

		static const Vec3 skRight;
		static const Vec3 skUp;
		static const Vec3 skForward;

	public:
		LOAD_CONSTRUCTOR(Vec3<T>)

		Vec3(T val = static_cast<T>(0));
		Vec3(T x, T y, T z);
		Vec3(const Vec3Holder<T>& kHolder);
		Vec3(const Vec3& kOther);
		~Vec3() = default;

		T norm() const;
		T sqrNorm() const;

		// Returns identity if norm is null
		Vec3 normalized() const;
		// Returns identity if any of Vec3.xyz is null 
		Vec3 inversed() const;
		Vec3 scaled(const Vec3& kRhs) const;

		// Does nothing if norm is null
		void normalize();
		// Does nothing if any of Vec3.xyz is null 
		void inverse();

		// Returns an angle in degree
		T angle(const Vec3& kOther) const;
		T dot(const Vec3& kOther) const;
		Vec3 cross(const Vec3& kOther) const;
		Vec3 clamp(const Vec3& kMin, const Vec3& kMax) const;
		
		Vec3Holder<T> data() const;

		Vec3 operator*(T rhs) const;
		// Returns identity if the value is null
		Vec3 operator/(T rhs) const;
		Vec3 operator+(const Vec3& kRhs) const;
		Vec3 operator-(const Vec3& kRhs) const;
		
		Vec3 operator-() const;
		Vec3& operator=(const Vec3& kRhs);
		Vec3& operator*=(T rhs);
		// Returns identity if the value is null
		Vec3& operator/=(T rhs);
		Vec3& operator+=(const Vec3& kRhs);
		Vec3& operator-=(const Vec3& kRhs);

		// Returns X if index is out of range
		T operator[](uint8_t index) const;
		// Returns X if index is out of range
		T& operator[](uint8_t index);

		bool operator==(const Vec3& kRhs) const;
		bool operator!=(const Vec3& kRhs) const;

		T mX;
		T mY;
		T mZ;
	};

	// Returns identity if one of Vec3.xyz is null
	template<typename T>
	Vec3<T> operator/(T lhs, const Vec3<T>& kRhs);

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vec3<T>& kRhs);

	typedef Vec3<uint32_t> Vec3ui;
	typedef Vec3<float> Vec3f;
	typedef Vec3<uint8_t> Vec3ui8;
}

META_CLASS(maj::math::Vec3f, 3u)
META_FIELD(maj::math::Vec3f, float, mX, 0u)
META_FIELD(maj::math::Vec3f, float, mY, 1u)
META_FIELD(maj::math::Vec3f, float, mZ, 2u)

META_CLASS(maj::math::Vec3ui, 3u)
META_FIELD(maj::math::Vec3ui, uint32_t, mX, 0u)
META_FIELD(maj::math::Vec3ui, uint32_t, mY, 1u)
META_FIELD(maj::math::Vec3ui, uint32_t, mZ, 2u)


META_CLASS(maj::math::Vec3ui8, 3u)
META_FIELD(maj::math::Vec3ui8, uint8_t, mX, 0u)
META_FIELD(maj::math::Vec3ui8, uint8_t, mY, 1u)
META_FIELD(maj::math::Vec3ui8, uint8_t, mZ, 2u)

namespace std
{
	template<typename T>
	struct hash<maj::math::Vec3Holder<T>>
	{
		size_t operator()(const maj::math::Vec3Holder<T>& kVec) const;
	};

	template<typename T>
	struct hash<maj::math::Vec3<T>>
	{
		size_t operator()(const maj::math::Vec3<T>& kVec) const;
	};
}

#include "Vec3.inl"

#endif