#ifndef _VEC2_H_
#define _VEC2_H_

#include <cstdint>
#include <iostream>
#include <ISerializable.h>

namespace maj::math
{
	template<typename T>
	struct Vec2Holder final
	{
		// Returns X if index is out of range
		T operator[](uint8_t index) const;
		// Returns X if index is out of range
		T& operator[](uint8_t index);

		bool operator==(const Vec2Holder& kOther) const;
		bool operator!=(const Vec2Holder& kOther) const;

		T mX;
		T mY;
	};

	template<typename T>
	class Vec2 final : public ISerializable
	{
		ENABLE_META(Vec2<T>)

	public:
		// Percent will be clamped to [0, 1]
		static Vec2 lerp(const Vec2& kStart, const Vec2& kEnd, T percent);
		// Percent will be clamped to [0, 1]
		static Vec2 slerp(Vec2 kStart, Vec2 kEnd, T percent);

		static const Vec2 skRight;
		static const Vec2 skUp;

	public:
		LOAD_CONSTRUCTOR(Vec2<T>)

		Vec2(T val = static_cast<T>(0));
		Vec2(T x, T y);
		Vec2(const Vec2Holder<T>& kHolder);
		Vec2(const Vec2& kOther);
		~Vec2() = default;

		T norm() const;
		T sqrNorm() const;

		// Returns identity if norm is null
		Vec2 normalized() const;
		// Returns identity if any of Vec2.xy is null 
		Vec2 inversed();
		Vec2 scaled(const Vec2& kRhs) const;

		// Does nothing if norm is null
		void normalize();
		// Does nothing if any of Vec2.xyz is null 
		void inverse();

		// Returns an angle in degree
		T angle(const Vec2& kOther) const;
		T dot(const Vec2& other) const;
		Vec2 clamp(const Vec2& kMin, const Vec2& kMax) const;
		
		Vec2Holder<T> data() const;

		Vec2 operator*(T rhs) const;
		// Returns identity if the value is null
		Vec2 operator/(T rhs) const;
		Vec2 operator+(const Vec2& kRhs) const;
		Vec2 operator-(const Vec2& kRhs) const;
		
		Vec2 operator-() const;
		Vec2& operator=(const Vec2& kRhs);
		Vec2& operator*=(T rhs);
		// Returns identity if the value is null
		Vec2& operator/=(T rhs);
		Vec2& operator+=(const Vec2& kRhs);
		Vec2& operator-=(const Vec2& kRhs);

		// Returns X if index is out of range
		T operator[](uint8_t index) const;
		// Returns X if index is out of range
		T& operator[](uint8_t index);

		bool operator==(const Vec2& kOther) const;
		bool operator!=(const Vec2& kOther) const;

		T mX;
		T mY;
	};

	// Returns identity if any of Vec2.xy is null
	template<typename T>
	Vec2<T> operator/(T lhs, const Vec2<T>& kRhs);

	template<typename T>
	std::ostream& operator<< (std::ostream& os, const Vec2<T>& kRhs);

	typedef Vec2<int32_t> Vec2i;
	typedef Vec2<uint32_t> Vec2ui;
	typedef Vec2<float> Vec2f;

}
META_CLASS(maj::math::Vec2i, 2u)
META_FIELD(maj::math::Vec2i, int32_t, mX, 0u)
META_FIELD(maj::math::Vec2i, int32_t, mY, 1u)

META_CLASS(maj::math::Vec2f, 2u)
META_FIELD(maj::math::Vec2f, float, mX, 0u)
META_FIELD(maj::math::Vec2f, float, mY, 1u)

META_CLASS(maj::math::Vec2ui, 2u)
META_FIELD(maj::math::Vec2ui, uint32_t, mX, 0u)
META_FIELD(maj::math::Vec2ui, uint32_t, mY, 1u)

namespace std
{
	template<typename T>
	struct hash<maj::math::Vec2Holder<T>>
	{
		size_t operator()(const maj::math::Vec2Holder<T>& kVec) const;
	};

	template<typename T>
	struct hash<maj::math::Vec2<T>>
	{
		size_t operator()(const maj::math::Vec2<T>& kVec) const;
	};
}

#include "Vec2.inl"

#endif
