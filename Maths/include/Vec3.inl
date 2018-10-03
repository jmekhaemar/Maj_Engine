#ifndef _VEC3_INL_
#define _VEC3_INL_

#include "Vec3.h"

#include <algorithm>

#include <Logging.h>

#include "Helper.h"

namespace maj::math
{
	template<typename T>
	DEFINE_HASH_OVERLOAD(Vec3<T>)

	template<typename T>
	T Vec3Holder<T>::operator[](uint8_t index) const
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		LOG(LogWarning, "Vec3Holder operator[] const : index out of range")
		return mX;
	}

	template<typename T>
	T& Vec3Holder<T>::operator[](uint8_t index)
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		LOG(LogWarning, "Vec3Holder operator[] : index out of range")
		return mX;
	}

	template<typename T>
	bool Vec3Holder<T>::operator==(const Vec3Holder<T>& kRhs) const
	{
		return Helper<T>::areSame(mX, kRhs.mX) && Helper<T>::areSame(mY, kRhs.mY) && Helper<T>::areSame(mZ, kRhs.mZ);
	}

	template<typename T>
	bool Vec3Holder<T>::operator!=(const Vec3Holder<T>& kRhs) const
	{
		return !Helper<T>::areSame(mX, kRhs.mX) || !Helper<T>::areSame(mY, kRhs.mY) || !Helper<T>::areSame(mZ, kRhs.mZ);
	}

	template<typename T>
	const Vec3<T> Vec3<T>::skRight { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) };

	template<typename T>
	const Vec3<T> Vec3<T>::skUp { static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) };

	template<typename T>
	const Vec3<T> Vec3<T>::skForward { static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) };

	template<typename T>
	Vec3<T> Vec3<T>::lerp(const Vec3<T>& kStart, const Vec3<T>& kEnd, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		return kStart * (static_cast<T>(1) - percent) + kEnd * percent;
	}

	template<typename T>
	Vec3<T> Vec3<T>::slerp(Vec3<T> kStart, Vec3<T> kEnd, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		T normStart { kStart.norm() };

		kStart.normalize();
		kEnd.normalize();

		T dot { kStart.dot(kEnd) };
		dot = std::clamp(dot, static_cast<T>(-1), static_cast<T>(1));

		T theta { std::acos(dot) * percent };
		Vec3<T> RelativeVec { kEnd - kStart * dot };
		RelativeVec.normalize();

		return ((kStart * std::cos(theta)) + (RelativeVec * std::sin(theta))) * normStart;
	}

	template<typename T>
	void Vec3<T>::initialize() {}

	template<typename T>
	Vec3<T>::Vec3(T val)
		: mX { val }, mY { val }, mZ { val }
	{}

	template<typename T>
	Vec3<T>::Vec3(T x, T y, T z)
		: mX { x }, mY { y }, mZ { z }
	{}

	template<typename T>
	Vec3<T>::Vec3(const Vec3Holder<T>& kOther)
		: mX{ kOther.mX }, mY{ kOther.mY }, mZ{ kOther.mZ }
	{}

	template<typename T>
	Vec3<T>::Vec3(const Vec3<T>& kOther)
		: mX { kOther.mX }, mY { kOther.mY }, mZ { kOther.mZ }
	{}
	
	template<typename T>
	T Vec3<T>::norm() const
	{
		return std::sqrt(mX * mX + mY * mY + mZ * mZ);
	}

	template<typename T>
	T Vec3<T>::sqrNorm() const
	{
		return mX * mX + mY * mY + mZ * mZ;
	}

	template<typename T>
	Vec3<T> Vec3<T>::normalized() const
	{
		T n{ norm() };
		if (Helper<T>::areSame(n, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec3 normalized : norm is null")
			return {};
		}

		T invN{ static_cast<T>(1) / n };
		return { mX * invN, mY * invN, mZ * invN };
	}

	template<typename T>
	Vec3<T> Vec3<T>::inversed() const
	{
		if (Helper<T>::areSame(mX, static_cast<T>(0)) || Helper<T>::areSame(mY, static_cast<T>(0)) ||
			Helper<T>::areSame(mZ, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec3 inversed : division by 0")
			return {};
		}

		return { static_cast<T>(1) / mX, static_cast<T>(1) / mY, static_cast<T>(1) / mZ };
	}
	
	template<typename T>
	Vec3<T> Vec3<T>::scaled(const Vec3<T>& kRhs) const
	{
		return { mX * kRhs.mX, mY * kRhs.mY, mZ * kRhs.mZ };
	}

	template<typename T>
	void Vec3<T>::normalize()
	{
		T n { norm() };
		if (Helper<T>::areSame(n, static_cast<T>(0)))
		{
			LOG(LogWarning, "vec3 normalize : norm is null")
			return;
		}

		T invN { static_cast<T>(1) / n };
		mX *= invN;
		mY *= invN;
		mZ *= invN;
	}

	template<typename T>
	void Vec3<T>::inverse()
	{
		if (Helper<T>::areSame(mX, static_cast<T>( 0 )) || Helper<T>::areSame(mY, static_cast<T>( 0 )) ||
			Helper<T>::areSame(mZ, static_cast<T>( 0 )))
		{
			LOG(LogWarning, "Vec3 inverse : division by 0 ")
				return;
		}
		mX = static_cast<T>( 1 ) / mX;
		mY = static_cast<T>( 1 ) / mY;
		mZ = static_cast<T>( 1 ) / mZ;
	}

	template<typename T>
	T Vec3<T>::angle(const Vec3<T>& kOther) const
	{
		if (operator==(kOther))
		{
			LOG(LogWarning, "Vec3 angle : angle between same Vec3")
			return static_cast<T>( 0 );
		}

		T n { sqrNorm() * kOther.sqrNorm() };
		if (Helper<T>::areSame(n, static_cast<T>( 0 )))
		{
			LOG(LogWarning, "Vec3 angle : division By 0")
			return static_cast<T>( 0 );
		}

		return std::acos(dot(kOther) / std::sqrt(n)) * RAD2DEG;
	}

	template<typename T>
	T Vec3<T>::dot(const Vec3<T>& kOther) const
	{
		return mX * kOther.mX + mY * kOther.mY + mZ * kOther.mZ;
	}

	template<typename T>
	Vec3<T> Vec3<T>::cross(const Vec3<T>& kOther) const
	{
		return { mY * kOther.mZ - mZ * kOther.mY,
				 mZ * kOther.mX - mX * kOther.mZ,
				 mX * kOther.mY - mY * kOther.mX };
	}
	
	template<typename T>
	Vec3<T> Vec3<T>::clamp(const Vec3<T>& kMin, const Vec3<T>& kMax) const
	{
		T x { std::clamp(mX, kMin.mX, kMax.mX) };
		T y { std::clamp(mY, kMin.mY, kMax.mY) };
		T z { std::clamp(mZ, kMin.mZ, kMax.mZ) };

		return { x, y, z };
	}

	template<typename T>
	Vec3Holder<T> Vec3<T>::data() const
	{
		return { mX, mY, mZ };
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator*(T rhs) const
	{
		return { mX * rhs, mY * rhs, mZ * rhs };
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator/(T rhs) const
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec3 operator/ : value is null")
			return {};
		}

		T invA{ static_cast<T>(1) / rhs };
		return { mX * invA, mY * invA, mZ * invA };
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator+(const Vec3<T>& kRhs) const
	{
		return { mX + kRhs.mX, mY + kRhs.mY, mZ + kRhs.mZ };
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator-(const Vec3<T>& kRhs) const
	{
		return { mX - kRhs.mX, mY - kRhs.mY, mZ - kRhs.mZ };
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator-() const
	{
		return { -mX, -mY, -mZ };
	}

	template<typename T>
	Vec3<T>& Vec3<T>::operator=(const Vec3<T>& kRhs)
	{
		mX = kRhs.mX;
		mY = kRhs.mY;
		mZ = kRhs.mZ;

		return *this;
	}

	template<typename T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& kRhs)
	{
		mX += kRhs.mX;
		mY += kRhs.mY;
		mZ += kRhs.mZ;

		return *this;
	}

	template<typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& kRhs)
	{
		mX -= kRhs.mX;
		mY -= kRhs.mY;
		mZ -= kRhs.mZ;

		return *this;
	}

	template<typename T>
	Vec3<T>& Vec3<T>::operator*=(T rhs)
	{
		mX *= rhs;
		mY *= rhs;
		mZ *= rhs;

		return *this;
	}

	template<typename T>
	Vec3<T>& Vec3<T>::operator/=(T rhs)
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec3 operator/= : value is null")
				return *this;
		}

		T invA{ static_cast<T>(1) / rhs };
		mX *= invA;
		mY *= invA;
		mZ *= invA;

		return *this;
	}

	template<typename T>
	T Vec3<T>::operator[](uint8_t index) const
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		LOG(LogWarning, "Vec3 operator[] const : index out of range")
		return mX;
	}

	template<typename T>
	T& Vec3<T>::operator[](uint8_t index)
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		LOG(LogWarning, "Vec3 operator[] : index out of range")
		return mX;
	}

	template<typename T>
	bool Vec3<T>::operator==(const Vec3<T>& kRhs) const
	{
		return Helper<T>::areSame(mX, kRhs.mX) && Helper<T>::areSame(mY, kRhs.mY) && Helper<T>::areSame(mZ, kRhs.mZ);
	}

	template<typename T>
	bool Vec3<T>::operator!=(const Vec3<T>& kRhs) const
	{
		return !Helper<T>::areSame(mX, kRhs.mX) || !Helper<T>::areSame(mY, kRhs.mY) || !Helper<T>::areSame(mZ, kRhs.mZ);
	}

	template<typename T>
	Vec3<T> operator/(T lhs, const Vec3<T>& kRhs)
	{
		if (Helper<T>::areSame(kRhs.mX, static_cast<T>(0)) || Helper<T>::areSame(kRhs.mY, static_cast<T>(0)) ||
			Helper<T>::areSame(kRhs.mZ, static_cast<T>(0)))
		{ 
			LOG(LogWarning, "Vec3 operator/(T lhs, const Vec3<T>& kRhs) : Vec3 is null")
			return {};
		}

		return { lhs / kRhs.mX, lhs / kRhs.mY, lhs / kRhs.mZ };
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vec3<T>& kRhs)
	{
		os << "Vec3(" << std::to_string(kRhs.mX) << ',' << std::to_string(kRhs.mY) << ',' << std::to_string(kRhs.mZ) << ')';
		return os;
	}
}

namespace std
{
	template <typename T>
	size_t hash<maj::math::Vec3Holder<T>>::operator()(const maj::math::Vec3Holder<T>& kVec) const
	{
		return ((hash<T>()(kVec.mX) ^
			(hash<T>()(kVec.mY) << 1)) >> 1) ^
			(hash<T>()(kVec.mZ) << 1);
	}

	template <typename T>
	size_t hash<maj::math::Vec3<T>>::operator()(const maj::math::Vec3<T>& kVec) const
	{
		return ((hash<T>()(kVec.mX) ^
				(hash<T>()(kVec.mY) << 1)) >> 1) ^
				(hash<T>()(kVec.mZ) << 1);
	}
}

#endif
