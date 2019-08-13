#ifndef _VEC2_INL_
#define _VEC2_INL_

#include "Vec2.h"

#include <algorithm>

#include <Logging.h>

#include "Helper.h"

namespace maj::math
{
	template<typename T>
	DEFINE_HASH_OVERLOAD(Vec2<T>)

	template<typename T>
	T Vec2Holder<T>::operator[](uint8_t index) const
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		LOG(LogWarning, "Vec2Holder operator[] const : index out of range")
		return mX;
	}

	template<typename T>
	T& Vec2Holder<T>::operator[](uint8_t index)
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		LOG(LogWarning, "Vec2Holder operator[] : index out of range")
		return mX;
	}

	template<typename T>
	bool Vec2Holder<T>::operator==(const Vec2Holder<T>& kOther) const
	{
		return Helper<T>::areSame(mX, kOther.mX) && Helper<T>::areSame(mY, kOther.mY);
	}

	template<typename T>
	bool Vec2Holder<T>::operator!=(const Vec2Holder<T>& kOther) const
	{
		return !Helper<T>::areSame(mX, kOther.mX) || !Helper<T>::areSame(mY, kOther.mY);
	}

	template<typename T>
	const Vec2<T> Vec2<T>::skRight{ static_cast<T>(1), static_cast<T>(0) };

	template<typename T>
	const Vec2<T> Vec2<T>::skUp{ static_cast<T>(0), static_cast<T>(1) };

	template<typename T>
	Vec2<T> Vec2<T>::lerp(const Vec2<T>& kStart, const Vec2<T>& kEnd, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		return kStart * (static_cast<T>(1) - percent) + kEnd * percent;
	}

	template<typename T>
	Vec2<T> Vec2<T>::slerp(Vec2<T> kStart, Vec2<T> kEnd, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		T normStart{ kStart.norm() };

		kStart.normalize();
		kEnd.normalize();

		T dot{ kStart.dot(kEnd) };
		dot = std::clamp(dot, static_cast<T>(-1), static_cast<T>(1));
		T theta{ std::acos(dot) * percent };
		Vec2<T> RelativeVec{ kEnd - kStart * dot };
		RelativeVec.normalize();

		return ((kStart * std::cos(theta)) + (RelativeVec * std::sin(theta))) * normStart;
	}
	
	template<typename T>
	void Vec2<T>::initialize() {}

	template<typename T>
	Vec2<T>::Vec2(T val)
		: mX{ val }, mY{ val }
	{}

	template<typename T>
	Vec2<T>::Vec2(T x, T y)
		: mX { x }, mY { y }
	{}

	template<typename T>
	Vec2<T>::Vec2(const Vec2Holder<T>& kHolder)
		: mX{ kHolder.mX }, mY{ kHolder.mY }
	{}

	template<typename T>
	Vec2<T>::Vec2(const Vec2<T>& kOther)
		: mX{ kOther.mX }, mY{ kOther.mY }
	{}

	template<typename T>
	T Vec2<T>::norm() const
	{
		return std::sqrt(mX * mX + mY * mY);
	}
	
	template<typename T>
	T Vec2<T>::sqrNorm() const
	{
		return mX * mX + mY * mY;
	}

	template<typename T>
	Vec2<T> Vec2<T>::normalized() const
	{
		T n{ norm() };
		if (Helper<T>::areSame(n, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec2 normalized : norm is null")
			return {};
		}

		T invN{ static_cast<T>(1) / n };
		return { mX  * invN, mY * invN };
	}
	
	template<typename T>
	Vec2<T> Vec2<T>::inversed()
	{
		if (Helper<T>::areSame(mX, static_cast<T>(0)) || Helper<T>::areSame(mY, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec2 inversed : one of Vec2.xy value is null")
			return {};
		}

		return { static_cast<T>(1) / mX, static_cast<T>(1) / mY};
	}

	template<typename T>
	Vec2<T> Vec2<T>::scaled(const Vec2<T>& rhs) const
	{
		return { mX * rhs.mX, mY * rhs.mY };
	}
	
	template<typename T>
	void Vec2<T>::normalize()
	{
		T n{ norm() };
		if (Helper<T>::areSame(n, 0.f))
		{
			LOG(LogWarning, "Vec2 normalize : norm is null")
			return;
		}
		T invN{ static_cast<T>(1) / n };
		mX *= invN;
		mY *= invN;
	}

	template<typename T>
	void Vec2<T>::inverse()
	{
		if (Helper<T>::areSame(mX, static_cast<T>(0)) || Helper<T>::areSame(mY, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec2 inverse : one of Vec2.xy value is null")
				return;
		}

		mX = static_cast<T>(1) / mX;
		mY = static_cast<T>(1) / mY;
	}

	template<typename T>
	T Vec2<T>::angle(const Vec2<T>& kOther) const
	{
		if (operator==(kOther))
		{
			LOG(LogWarning, "Vec2 angle : angle between same Vec2")
				return static_cast<T>(0);
		}

		T n { sqrNorm() * kOther.sqrNorm()};
		if (Helper<T>::areSame(n, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec2 angle : division by 0")
			return static_cast<T>(0);
		}

		return std::acos(dot(kOther) / std::sqrt(n)) * RAD2DEG;
	}

	template<typename T>
	T Vec2<T>::dot(const Vec2<T>& kOther) const
	{
		return mX * kOther.mX + mY * kOther.mY;
	}

	template<typename T>
	Vec2<T> Vec2<T>::clamp(const Vec2<T>& kMin, const Vec2<T>& kMax) const
	{
		T x { std::clamp(mX, kMin.mX, kMax.mX) };
		T y { std::clamp(mY, kMin.mY, kMax.mY) };

		return { x, y };
	}
	
	template<typename T>
	Vec2Holder<T> Vec2<T>::data() const
	{
		return { mX, mY };
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator*(T rhs) const
	{
		return { mX * rhs, mY * rhs};
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator/(T rhs) const
	{
		if (Helper<T>::areSame(rhs , static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec2 operator/ : division by 0")
			return {};
		}

		T invA{ static_cast<T>(1) / rhs };
		return { mX * invA, mY * invA };
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator+(const Vec2<T>& rhs) const
	{
		return { mX + rhs.mX, mY + rhs.mY};
	}
	
	template<typename T>
	Vec2<T> Vec2<T>::operator-(const Vec2<T>& rhs) const
	{
		return { mX - rhs.mX, mY - rhs.mY};
	}
	
	template<typename T>
	Vec2<T> Vec2<T>::operator-() const
	{
		return { -mX, -mY};
	}

	template<typename T>
	Vec2<T>&  Vec2<T>::operator=(const Vec2<T>& rhs)
	{
		mX = rhs.mX;
		mY = rhs.mY;

		return *this;
	}

	template<typename T>
	Vec2<T>&  Vec2<T>::operator*=(T rhs)
	{
		mX *= rhs;
		mY *= rhs;

		return *this;
	}

	template<typename T>
	Vec2<T>& Vec2<T>::operator/=(T rhs)
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec2 operator/= : division by 0")
			return *this;
		}

		T invA { static_cast<T>(1) / rhs };
		mX *= invA;
		mY *= invA;

		return *this;
	}

	template<typename T>
	Vec2<T>&  Vec2<T>::operator+=(const Vec2<T>& rhs) 
	{
		mX += rhs.mX;
		mY += rhs.mY;

		return *this;
	}
	
	template<typename T>
	Vec2<T>&  Vec2<T>::operator-=(const Vec2<T>& rhs)
	{
		mX -= rhs.mX;
		mY -= rhs.mY;

		return *this;
	}
	
	template<typename T>
	T Vec2<T>::operator[](uint8_t index) const
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		LOG(LogWarning, "Vec2 operator[] const : index out of range")
		return mX;
	}

	template<typename T>
	T& Vec2<T>::operator[](uint8_t index)
	{
		if (index == 0u)
			return mX;

		if (index == 1u)
			return mY;

		LOG(LogWarning, "Vec2 operator[] : index out of range")
		return mX;
	}

	template<typename T>
	bool Vec2<T>::operator==(const Vec2<T>& kOther) const
	{
		return Helper<T>::areSame(mX, kOther.mX) && Helper<T>::areSame(mY, kOther.mY);
	}

	template<typename T>
	bool Vec2<T>::operator!=(const Vec2<T>& kOther) const
	{
		return !Helper<T>::areSame(mX, kOther.mX) || !Helper<T>::areSame(mY, kOther.mY);
	}

	template<typename T>
	Vec2<T> operator/(T lhs, const Vec2<T>& kRhs)
	{
		if (Helper<T>::areSame(kRhs.mX, static_cast<T>(0)) || Helper<T>::areSame(kRhs.mY, static_cast<T>(0)))
		{
			LOG(LogWarning, "Vec2 operator/(T lhs, const Vec2<T>& kRhs) : Vec2 is null")
				return {};
		}

		return { lhs / kRhs.mX, lhs / kRhs.mY};
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vec2<T>& kRhs)
	{
		os << "Vec2(" << std::to_string(kRhs.mX) << ',' << std::to_string(kRhs.mY) << ')';
		return os;
	}
}

namespace std
{
	template<typename T>
	size_t hash<maj::math::Vec2Holder<T>>::operator()(const maj::math::Vec2Holder<T>& kVec) const
	{
		return ((hash<T>()(kVec.mX) ^
			(hash<T>()(kVec.mY) << 1)) >> 1);
	}

	template<typename T>
	size_t hash<maj::math::Vec2<T>>::operator()(const maj::math::Vec2<T>& kVec) const
	{
		return ((hash<T>()(kVec.mX) ^
				(hash<T>()(kVec.mY) << 1)) >> 1);
	}
}

#endif
