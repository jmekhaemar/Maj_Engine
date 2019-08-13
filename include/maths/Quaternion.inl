#ifndef _QUATERNION_INL_
#define _QUATERNION_INL_

#include "Quaternion.h"

#include <algorithm>

#include <Logging.h>

#include "Helper.h"

namespace maj::math
{
	template <typename T>
	DEFINE_HASH_OVERLOAD(Quaternion<T>)

	template <typename T>
	const Quaternion<T> Quaternion<T>::skIdentity { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) };

	template<typename T>
	Quaternion<T> Quaternion<T>::lookRotation(const Vec3<T>& kForward, const Vec3<T>& kUp)
	{
		// Calculate base
		Vec3<T> zaxis { kForward.normalized() };
		Vec3<T> xaxis { kUp.cross(zaxis) };

		if (xaxis == Vec3<T>{})
			return {};

		xaxis.normalize();
		Vec3<T> yaxis { zaxis.cross(xaxis) };

		// Create column-major matrix
		T m00 { xaxis.mX };
		T m01 { xaxis.mY };
		T m02 { xaxis.mZ };

		T m10 { yaxis.mX };
		T m11 { yaxis.mY };
		T m12 { yaxis.mZ };

		T m20 { zaxis.mX };
		T m21 { zaxis.mY };
		T m22 { zaxis.mZ };

		// Convert matrix to quaternion
		Quaternion<T> q {};

		T trace { m00 + m11 + m22 };
		if (trace > static_cast<T>(0))
		{
			T num (std::sqrt(trace + static_cast<T>(1)));
			q.mW = num * static_cast<T>(0.5);

			num = static_cast<T>(0.5) / num;
			q.mX = (m12 - m21) * num;
			q.mY = (m20 - m02) * num;
			q.mZ = (m01 - m10) * num;
			return q;
		}
		
		// If m[0][0] is the biggest value
		if ((m00 >= m11) && (m00 >= m22))
		{
			T num (std::sqrt(static_cast<T>(1) + m00 - m11 - m22));
			q.mX = static_cast<T>(0.5) * num;

			num = static_cast<T>(0.5) / num;
			q.mW = (m12 - m21) * num;
			q.mY = (m01 + m10) * num;
			q.mZ = (m20 + m02) * num;
			return q;
		}
		
		// If m[1][1] is the biggest value
		if (m11 > m22)
		{
			T num (std::sqrt(static_cast<T>(1) + m11- m00 - m22));
			q.mY = num * static_cast<T>(0.5);

			num = static_cast<T>(0.5) / num;
			q.mW = (m20 - m02) * num;
			q.mX = (m01 + m10) * num;
			q.mZ = (m12 + m21) * num;
			return q;
		}

		// m[2][2] is the biggest value
		T num (std::sqrt(static_cast<T>(1) + m22 - m00 - m11));
		q.mZ = static_cast<T>(0.5) * num;

		num = static_cast<T>(0.5) / num;
		q.mW = (m01 - m10) * num;
		q.mX = (m20 + m02) * num;
		q.mY = (m12 + m21) * num;

		return q;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::fromEuler(const Vec3<T>& kEuler)
	{
		Vec3<T> half { kEuler * DEG2RAD * static_cast<T>(0.5) };
		Vec3<T> c { std::cos(half.mX), std::cos(half.mY), std::cos(half.mZ) };
		Vec3<T> s { std::sin(half.mX), std::sin(half.mY), std::sin(half.mZ) };

		T w { c.mX * c.mY * c.mZ + s.mX * s.mY * s.mZ };
		T x { s.mX * c.mY * c.mZ - c.mX * s.mY * s.mZ };
		T y { c.mX * s.mY * c.mZ + s.mX * c.mY * s.mZ };
		T z { c.mX * c.mY * s.mZ - s.mX * s.mY * c.mZ };

		return { w, x, y, z };
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::fromEuler(T eulerX, T eulerY, T eulerZ)
	{
		return fromEuler({ eulerX, eulerY, eulerZ });
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::lerp(const Quaternion<T>& start, const Quaternion<T>& end, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		return (start * (static_cast<T>(1) - percent) + end * percent);
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::nlerp(const Quaternion<T>& start, const Quaternion<T>& end, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		return (start * (static_cast<T>(1) - percent) + end * percent).normalized();
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::slerp(Quaternion<T> start, Quaternion<T> end, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		// Cf Visualizing Quaternions by Andrew Hanson : Table E.5
		start.normalize();
		end.normalize();

		T beta { static_cast<T>(0) }, alpha { percent };
		bool bFlip { false };
		T cosT { start.dot(end) };

		if (cosT < static_cast<T>(0))
		{
			cosT = -cosT;
			bFlip = true;
		}

		if (Helper<T>::areSame(static_cast<T>(1), cosT))
			beta = static_cast<T>(1) - alpha;

		else
		{
			T theta { std::acos(cosT) };
			T sinT { std::sin(theta) };

			beta = sin(theta - alpha * theta) / sinT;
			alpha = sin(alpha*theta) / sinT;
		}

		if (bFlip)
			alpha = -alpha;

		return start * beta + end * alpha;
	}

	template<typename T>
	void Quaternion<T>::initialize() {}

	template<typename T>
	Quaternion<T>::Quaternion()
		: mW { static_cast<T>(1) }, mX { static_cast<T>(0) }, mY { static_cast<T>(0) }, mZ { static_cast<T>(0) }
	{}

	template<typename T>
	Quaternion<T>::Quaternion(T w, T x, T y, T z)
		: mW { w }, mX { x }, mY { y }, mZ { z }
	{}

	template<typename T>
	Quaternion<T>::Quaternion(const Quaternion& kOther)
		: mW { kOther.mW }, mX { kOther.mX }, mY { kOther.mY }, mZ { kOther.mZ }
	{}

	template<typename T>
	Quaternion<T>::Quaternion(T angle, const Vec3<T>& kAxis)
		: mW { std::cos(angle * DEG2RAD * static_cast<T>(0.5)) }, mX { kAxis.mX }, mY { kAxis.mY }, mZ { kAxis.mZ }
	{
		T norm { std::sqrt(mX * mX + mY * mY + mZ * mZ) };
		if (Helper<T>::areSame(norm, static_cast<T>(0)))
		{ 
			LOG(LogWarning, "Quaternion angle/axis : axis is null")
			return;
		}
			
		T temp { std::sin(angle * DEG2RAD * static_cast<T>(0.5)) / norm };
		mX *= temp;
		mY *= temp;
		mZ *= temp;
	}

	template<typename T>
	T Quaternion<T>::modulus() const
	{
		return std::sqrt(mW * mW + mX * mX + mY * mY + mZ * mZ);
	}

	template<typename T>
	T Quaternion<T>::sqrModulus() const
	{
		return mW * mW + mX * mX + mY * mY + mZ * mZ;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::conjugated() const
	{
		return { mW, -mX, -mY, -mZ };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::inversed() const
	{
		T m { sqrModulus() };
		if (Helper<T>::areSame(m, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion inversed : modulus is null")
			return {};
		}

		T invM { static_cast<T>(1) / m };
		return { mW * invM, -mX * invM, -mY * invM, -mZ * invM };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::normalized() const
	{
		T m { modulus() };
		if (Helper<T>::areSame(m, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion normalized : modulus is null")
			return {};
		}

		T invM { static_cast<T>(1) / m };
		return { mW * invM, mX * invM, mY * invM, mZ * invM };
	}

	template<typename T>
	void Quaternion<T>::conjugate()
	{
		mX = -mX;
		mY = -mY;
		mZ = -mZ;
	}

	template<typename T>
	void Quaternion<T>::inverse()
	{
		T m { sqrModulus() };
		if (Helper<T>::areSame(m, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion inverse : modulus is null")
			return;
		}

		T invM { static_cast<T>(1) / m };

		mW *= invM;
		mX *= -invM;
		mY *= -invM;
		mZ *= -invM;
	}

	template<typename T>
	void Quaternion<T>::normalize()
	{
		T m{ modulus() };
		if (Helper<T>::areSame(m, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion normalize : modulus is null")
			return;
		}

		T invM { static_cast<T>(1) / m };

		mW *= invM;
		mX *= invM;
		mY *= invM;
		mZ *= invM;
	}

	template<typename T>
	T Quaternion<T>::dot(const Quaternion<T>& kOther) const
	{
		return mW * kOther.mW + mX * kOther.mX + mY * kOther.mY + mZ * kOther.mZ;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::clamp(const Quaternion<T>& kMin, const Quaternion<T>& kMax) const
	{
		T w { std::clamp(mW, kMin.mW, kMax.mW) };
		T x { std::clamp(mX, kMin.mX, kMax.mX) };
		T y { std::clamp(mY, kMin.mY, kMax.mY) };
		T z { std::clamp(mZ, kMin.mZ, kMax.mZ) };

		return { w, x, y, z };
	}

	template<typename T>
	std::string Quaternion<T>::toString() const
	{
		return "Quaternion(" + std::to_string(mW) + ',' + std::to_string(mX) + ',' + std::to_string(mY) + ',' + std::to_string(mZ) + ')';
	}

	template <typename T>
	Vec3<T> Quaternion<T>::toEuler() const
	{
		Quaternion<T> q {normalized()};

		Vec3<T> euler {};
		T y2 { q.mY * q.mY };

		// Roll
		T sinR(static_cast<T>(2) * ( q.mW * q.mX + q.mY * q.mZ ));
		T cosR(static_cast<T>(1) - static_cast<T>(2) * ( q.mX * q.mX + y2 ));
		euler.mX = std::atan2(sinR, cosR);

		// Pitch
		T sinP(static_cast<T>(2) * (q.mW * q.mY - q.mZ * q.mX));
		euler.mY = std::fabs(sinP) < static_cast<T>(1) ? std::asin(sinP) : std::copysign(PI * static_cast<T>(0.5), sinP);

		// Yaw
		T sinY(static_cast<T>(2) * (q.mW * q.mZ + q.mX * q.mY ));
		T cosY(static_cast<T>(1) - static_cast<T>(2) * ( y2 + q.mZ * q.mZ ));
		euler.mZ = std::atan2(sinY, cosY);

		return euler * RAD2DEG;
	}

	template <typename T>
	Vec3<T> Quaternion<T>::toAxis() const
	{
		T w2 { mW * mW };
		if (w2 >= static_cast<T>(1))
		{
			LOG(LogWarning, "Quaternion toAxis : square root of 0 or negative value")

			// Could return any axis, we chose to do it like glm
			return { static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) };
		}

		T inv { static_cast<T>(1) / std::sqrt(static_cast<T>(1) - w2) };
		return { mX * inv, mY * inv, mZ * inv };
	}

	template <typename T>
	T Quaternion<T>::toAngle() const
	{
		return static_cast<T>(2) * std::acos(mW) * RAD2DEG;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::operator-() const
	{
		return { -mW, -mX, -mY, -mZ };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::operator*(T rhs) const
	{
		return { mW * rhs, mX * rhs, mY * rhs, mZ * rhs };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::operator/(T rhs) const
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion operator/ : division by 0")
			return {};
		}

		T invRhs { static_cast<T>(1) / rhs };
		return { mW * invRhs, mX * invRhs, mY * invRhs, mZ * invRhs };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::operator+(const Quaternion<T>& kRhs) const
	{
		return { mW + kRhs.mW, mX + kRhs.mX, mY + kRhs.mY, mZ + kRhs.mZ };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::operator-(const Quaternion<T>& kRhs) const
	{
		return { mW - kRhs.mW, mX - kRhs.mX, mY - kRhs.mY, mZ - kRhs.mZ };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& kRhs) const
	{
		T w { mW * kRhs.mW - mX * kRhs.mX - mY * kRhs.mY - mZ * kRhs.mZ };
		T x { mW * kRhs.mX + mX * kRhs.mW + mY * kRhs.mZ - mZ * kRhs.mY };
		T y { mW * kRhs.mY + mY * kRhs.mW + mZ * kRhs.mX - mX * kRhs.mZ };
		T z { mW * kRhs.mZ + mZ * kRhs.mW + mX * kRhs.mY - mY * kRhs.mX };

		return { w, x, y, z };
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::operator/(const Quaternion<T>& kRhs) const
	{
		T m{ kRhs.sqrModulus() };
		if (Helper<T>::areSame(m, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion operator/ : quaternion is null")
			return {};
		}

		T invM { static_cast<T>(1) / m };

		T rW { kRhs.mW * invM };
		T rX { -kRhs.mX * invM };
		T rY { -kRhs.mY * invM };
		T rZ { -kRhs.mZ * invM };
		
		T w { mW * rW - mX * rX - mY * rY - mZ * rZ };
		T x { mW * rX + mX * rW + mY * rZ - mZ * rY };
		T y { mW * rY + mY * rW + mZ * rX - mX * rZ };
		T z { mW * rZ + mZ * rW + mX * rY - mY * rX };

		return { w, x, y, z };
	}

	template<typename T>
	Vec3<T> Quaternion<T>::operator*(const Vec3<T>& kRhs) const
	{
		Vec3<T> quatVec{ mX, mY, mZ };
		Vec3<T> uv{ quatVec.cross(kRhs) };
		Vec3<T> uuv{ quatVec.cross(uv) };

		uv *= static_cast<T>(2) * mW;
		uuv *= static_cast<T>(2);

		return kRhs + uv + uuv;
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::operator=(const Quaternion<T>& kRhs)
	{
		mW = kRhs.mW;
		mX = kRhs.mX;
		mY = kRhs.mY;
		mZ = kRhs.mZ;

		return *this;
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::operator*=(T rhs)
	{
		mW *= rhs;
		mX *= rhs;
		mY *= rhs;
		mZ *= rhs;

		return *this;
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::operator/=(T rhs)
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion operator/= : division by 0")
			return *this;
		}

		T inv { static_cast<T>(1) / rhs };
		mW *= inv;
		mX *= inv;
		mY *= inv;
		mZ *= inv;

		return *this;
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& rhs)
	{
		mW += rhs.mW;
		mX += rhs.mX;
		mY += rhs.mY;
		mZ += rhs.mZ;

		return *this;
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<T>& kRhs)
	{
		mW -= kRhs.mW;
		mX -= kRhs.mX;
		mY -= kRhs.mY;
		mZ -= kRhs.mZ;

		return *this;
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& kRhs)
	{
		T w{ mW * kRhs.mW - mX * kRhs.mX - mY * kRhs.mY - mZ * kRhs.mZ };
		T x{ mW * kRhs.mX + mX * kRhs.mW + mY * kRhs.mZ - mZ * kRhs.mY };
		T y{ mW * kRhs.mY - mX * kRhs.mZ + mY * kRhs.mW + mZ * kRhs.mX };
		T z{ mW * kRhs.mZ + mX * kRhs.mY - mY * kRhs.mX + mZ * kRhs.mW };

		mW = w;
		mX = x;
		mY = y;
		mZ = z;

		return *this;
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::operator/=(const Quaternion<T>& kRhs)
	{
		T m { kRhs.sqrModulus() };
		if (Helper<T>::areSame(m, static_cast<T>(0)))
		{
			LOG(LogWarning, "Quaternion operator/= : quaternion is null")
			return *this;
		}

		T invM { static_cast<T>(1) / m };

		T rW { kRhs.mW * invM };
		T rX { -kRhs.mX * invM };
		T rY { -kRhs.mY * invM };
		T rZ { -kRhs.mZ * invM };

		T w { mW * rW - mX * rX - mY * rY - mZ * rZ };
		T x { mW * rX + mX * rW + mY * rZ - mZ * rY };
		T y { mW * rY + mY * rW + mZ * rX - mX * rZ };
		T z { mW * rZ + mZ * rW + mX * rY - mY * rX };

		mW = w;
		mX = x;
		mY = y;
		mZ = z;

		return *this;
	}

	template<typename T>
	bool Quaternion<T>::operator==(const Quaternion<T>& kRhs) const
	{
		return Helper<T>::areSame(mW, kRhs.mW) && Helper<T>::areSame(mX, kRhs.mX) && Helper<T>::areSame(mY, kRhs.mY) && Helper<T>::areSame(mZ, kRhs.mZ);
	}

	template<typename T>
	bool Quaternion<T>::operator!=(const Quaternion<T>& kRhs) const
	{
		return !Helper<T>::areSame(mW, kRhs.mW) || !Helper<T>::areSame(mX, kRhs.mX) || !Helper<T>::areSame(mY, kRhs.mY) || !Helper<T>::areSame(mZ, kRhs.mZ);
	}
}

#endif
