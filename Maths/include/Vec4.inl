#ifndef _VEC4_INL_
#define _VEC4_INL_

#include "Vec4.h"

#include <algorithm>

#include <Logging.h>

#include "Helper.h"

namespace maj::math
{
	template<typename T>
	DEFINE_HASH_OVERLOAD(Vec4<T>)

	template<typename T>
	T Vec4Holder<T>::operator[](uint8_t index) const
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		if (index == 3u)
			return mW;

		LOG(LogWarning, "Vec4Holder operator[] const : index out of range")
		return mX;
	}

	template<typename T>
	T& Vec4Holder<T>::operator[](uint8_t index)
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		if (index == 3u)
			return mW;

		LOG(LogWarning, "Vec4Holder operator[] : index out of range")
		return mX;
	}

	template<typename T>
	bool Vec4Holder<T>::operator==(const Vec4Holder<T>& kRhs) const
	{
		return	Helper<T>::areSame(mX, kRhs.mX) && Helper<T>::areSame(mY, kRhs.mY) &&
			Helper<T>::areSame(mZ, kRhs.mZ) && Helper<T>::areSame(mW, kRhs.mW);
	}

	template<typename T>
	bool Vec4Holder<T>::operator!=(const Vec4Holder<T>& kRhs) const
	{
		return !Helper<T>::areSame(mX, kRhs.mX) || !Helper<T>::areSame(mY, kRhs.mY) ||
			!Helper<T>::areSame(mZ, kRhs.mZ) || !Helper<T>::areSame(mW, kRhs.mW);
	}

	template<typename T>
	const Vec4<T> Vec4<T>::skRight { static_cast<T>(1),static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) };

	template<typename T>
	const Vec4<T> Vec4<T>::skUp { static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) };

	template<typename T>
	const Vec4<T> Vec4<T>::skForward { static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) };

	template<typename T>
	Vec4<T> Vec4<T>::lerp(const Vec4<T>& kStart, const Vec4<T>& kEnd, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		return kStart * (static_cast<T>(1) - percent) + kEnd * percent;
	}

	template<typename T>
	Vec4<T> Vec4<T>::slerp(Vec4<T> kStart, Vec4<T> kEnd, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		T normStart { kStart.norm() };

		kStart.normalize();
		kEnd.normalize();

		T dot { kStart.dot(kEnd) };
		dot = std::clamp(dot, static_cast<T>(-1), static_cast<T>(1));

		T theta { std::acos(dot) * percent };
		Vec4<T> RelativeVec{ kEnd - kStart * dot };
		RelativeVec.normalize();

		return ((kStart * std::cos(theta)) + (RelativeVec * std::sin(theta))) * normStart;
	}

	template<typename T>
	void Vec4<T>::initialize() {}

	template<typename T>
	Vec4<T>::Vec4(T val)
		: mX { val }, mY { val }, mZ { val }, mW { val }
	{}

	template<typename T>
	Vec4<T>::Vec4(T x, T y, T z, T w)
		: mX { x }, mY { y }, mZ { z }, mW { w }
	{}

	template<typename T>
	Vec4<T>::Vec4(const Vec4Holder<T>& kOther)
		: mX{ kOther.mX }, mY{ kOther.mY }, mZ{ kOther.mZ }, mW{ kOther.mW }
	{}

	template<typename T>
	Vec4<T>::Vec4(const Vec4<T>& kOther)
		: mX { kOther.mX }, mY { kOther.mY }, mZ { kOther.mZ }, mW { kOther.mW }
	{}

	template<typename T>
	T Vec4<T>::norm() const
	{
		return std::sqrt(mX * mX + mY * mY + mZ * mZ + mW * mW);
	}

	template<typename T>
	T Vec4<T>::sqrNorm() const
	{
		return mX * mX + mY * mY + mZ * mZ + mW * mW;
	}

	template<typename T>
	Vec4<T> Vec4<T>::normalized() const
	{
		T n{ norm() };
		if (Helper<T>::areSame(n, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 normalized : norm is null")
			return {};
		}

		T invN{ static_cast<T>(1) / n };
		return { mX * invN, mY * invN, mZ * invN, mW * invN };
	}

	template<typename T>
	Vec4<T> Vec4<T>::inversed() const
	{
		if (Helper<T>::areSame(mX, static_cast<T>(0)) || Helper<T>::areSame(mY, static_cast<T>(0)) ||
			Helper<T>::areSame(mZ, static_cast<T>(0)) || Helper<T>::areSame(mW, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 inversed : division by 0")
			return {};
		}

		return { static_cast<T>(1) / mX, static_cast<T>(1) / mY, static_cast<T>(1) / mZ, static_cast<T>(1) / mW };
	}

	template<typename T>
	Vec4<T> Vec4<T>::homogenized() const
	{
		if (Helper<T>::areSame(mW, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 homogenized : w is null")
			return {};
		}
		
		T invW { static_cast<T>(1) / mW };
		return { mX * invW, mY * invW, mZ * invW, static_cast<T>(1) };
	}
	
	template<typename T>
	Vec4<T> Vec4<T>::scaled(const Vec4<T>& kRhs) const
	{
		return { mX * kRhs.mX, mY * kRhs.mY, mZ * kRhs.mZ, mW * kRhs.mW };
	}

	template<typename T>
	void Vec4<T>::normalize()
	{
		T n { norm() };
		if (Helper<T>::areSame(n, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 normalize : norm is null")
			return;
		}

		T invN{ static_cast<T>(1) / n };
		mX *= invN;
		mY *= invN;
		mZ *= invN;
		mW *= invN;
	}

	template<typename T>
	void Vec4<T>::inverse()
	{
		if (Helper<T>::areSame(mX, static_cast<T>(0)) || Helper<T>::areSame(mY, static_cast<T>(0)) ||
			Helper<T>::areSame(mZ, static_cast<T>(0)) || Helper<T>::areSame(mW, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 inverse : division by 0")
			return;
		}

		mX = static_cast<T>(1) / mX;
		mY = static_cast<T>(1) / mY;
		mZ = static_cast<T>(1) / mZ;
		mW = static_cast<T>(1) / mW;
	}

	template<typename T>
	void Vec4<T>::homogenize()
	{
		if (Helper<T>::areSame(mW, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 homogenize : w is null")
			return;
		}

		T invW { static_cast<T>(1) / mW };

		mX *= invW;
		mY *= invW;
		mZ *= invW;
		mW = static_cast<T>(1);
	}

	template<typename T>
	T Vec4<T>::angle(const Vec4<T>& kOther) const
	{
		if (operator==(kOther))
		{
			LOG(LogWarning, "Vec4 angle : angle between same Vec4")
			return static_cast<T>(0);
		}

		T n { sqrNorm() * kOther.sqrNorm() };
		if (Helper<T>::areSame(n, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 angle : division By 0")
			return static_cast<T>(0);
		}

		return std::acos(dot(kOther) / std::sqrt(n)) * RAD2DEG;
	}

	template<typename T>
	T Vec4<T>::dot(const Vec4<T>& kOther) const
	{
		return mX * kOther.mX + mY * kOther.mY + mZ * kOther.mZ + mW * kOther.mW;
	}

	template<typename T>
	Vec4<T> Vec4<T>::clamp(const Vec4<T>& kMin, const Vec4<T>& kMax) const
	{
		T x { std::clamp(mX, kMin.mX, kMax.mX) };
		T y { std::clamp(mY, kMin.mY, kMax.mY) };
		T z { std::clamp(mZ, kMin.mZ, kMax.mZ) };
		T w { std::clamp(mW, kMin.mW, kMax.mW) };

		return { x, y, z, w};
	}

	template <typename T>
	Vec4Holder<T> Vec4<T>::data() const
	{
		return { mX, mY, mZ, mW };
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator*(T rhs) const
	{
		return { mX * rhs, mY * rhs, mZ * rhs, mW * rhs };
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator/(T rhs) const
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 operator/ : division by 0")
			return {};
		}

		T invA{ static_cast<T>(1) / rhs };
		return { mX * invA, mY * invA, mZ * invA, mW * invA };
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator+(const Vec4<T>& kRhs) const
	{
		return { mX + kRhs.mX, mY + kRhs.mY, mZ + kRhs.mZ, mW + kRhs.mW };
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator-(const Vec4<T>& kRhs) const
	{
		return { mX - kRhs.mX, mY - kRhs.mY, mZ - kRhs.mZ, mW - kRhs.mW };
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator-() const
	{
		return { -mX, -mY, -mZ, -mW };
	}

	template<typename T>
	Vec4<T>& Vec4<T>::operator=(const Vec4<T>& kRhs)
	{
		mX = kRhs.mX;
		mY = kRhs.mY;
		mZ = kRhs.mZ;
		mW = kRhs.mW;

		return *this;
	}

	template<typename T>
	Vec4<T>& Vec4<T>::operator*=(T rhs)
	{
		mX *= rhs;
		mY *= rhs;
		mZ *= rhs;
		mW *= rhs;

		return *this;
	}

	template<typename T>
	Vec4<T>& Vec4<T>::operator/=(T rhs)
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 operator/= : division by 0")
			return *this;
		}

		T inv { static_cast<T>(1) / rhs };

		mX *= inv;
		mY *= inv;
		mZ *= inv;
		mW *= inv;

		return *this;
	}

	template<typename T>
	Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& kRhs)
	{
		mX += kRhs.mX;
		mY += kRhs.mY;
		mZ += kRhs.mZ;
		mW += kRhs.mW;

		return *this;
	}

	template<typename T>
	Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& kRhs)
	{
		mX -= kRhs.mX;
		mY -= kRhs.mY;
		mZ -= kRhs.mZ;
		mW -= kRhs.mW;

		return *this;
	}

	template<typename T>
	T Vec4<T>::operator[](uint8_t index) const
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		if (index == 3u)
			return mW;
		
		LOG(LogWarning, "Vec4 operator[] const : index out of range")
		return mX;
	}

	template<typename T>
	T& Vec4<T>::operator[](uint8_t index)
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		if (index == 2u)
			return mZ;

		if (index == 3u)
			return mW;

		LOG(LogWarning, "Vec4 operator[] : index out of range")
		return mX;
	}

	template<typename T>
	bool Vec4<T>::operator==(const Vec4<T>& kRhs) const
	{
		return	Helper<T>::areSame(mX, kRhs.mX) && Helper<T>::areSame(mY, kRhs.mY) &&
				Helper<T>::areSame(mZ, kRhs.mZ) && Helper<T>::areSame(mW, kRhs.mW);
	}

	template<typename T>
	bool Vec4<T>::operator!=(const Vec4<T>& kRhs) const
	{
		return !Helper<T>::areSame(mX, kRhs.mX) || !Helper<T>::areSame(mY, kRhs.mY) ||
			   !Helper<T>::areSame(mZ, kRhs.mZ) || !Helper<T>::areSame(mW, kRhs.mW);
	}

	template<typename T>
	Vec4<T> operator/(T lhs, const Vec4<T>& kRhs)
	{
		if (Helper<T>::areSame(kRhs.mX, static_cast<T>(0)) || Helper<T>::areSame(kRhs.mY, static_cast<T>(0)) ||
			Helper<T>::areSame(kRhs.mZ, static_cast<T>(0)) || Helper<T>::areSame(kRhs.mW, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec4 operator/(T lhs, const Vec4<T>& kRhs) : Vec4 is null")
			return {};
		}

		return { lhs / kRhs.mX, lhs / kRhs.mY, lhs / kRhs.mZ, lhs / kRhs.mW };
	}

	template<typename T>
	std::ostream& operator<< (std::ostream& os, const Vec4<T>& kRhs)
	{
		os << "Vec4(" << std::to_string(kRhs.mX) << ',' << std::to_string(kRhs.mY) << ',' << std::to_string(kRhs.mZ) << ',' << std::to_string(kRhs.mW) << ')';
		return os;
	}
}

namespace std
{
	template<typename T>
	size_t hash<maj::math::Vec4Holder<T>>::operator()(const maj::math::Vec4Holder<T>& kVec) const
	{
		return ((hash<T>()(kVec.mX) ^
			(hash<T>()(kVec.mY) << 1)) >> 1) ^
			((hash<T>()(kVec.mZ) << 1) ^
			(hash<T>()(kVec.mW) << 1) >> 1);
	}

	template<typename T>
	size_t hash<maj::math::Vec4<T>>::operator()(const maj::math::Vec4<T>& kVec) const
	{
		return ((hash<T>()(kVec.mX) ^
				(hash<T>()(kVec.mY) << 1)) >> 1) ^
				((hash<T>()(kVec.mZ) << 1) ^
				(hash<T>()(kVec.mW) << 1) >> 1) ;
	}
}

#endif
