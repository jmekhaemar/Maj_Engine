#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Vec3.h"

namespace maj::math
{
	template <typename T>
	class Quaternion final : public ISerializable
	{
		ENABLE_META(Quaternion<T>)

	public:
		// Returns identity if forward or up is null or if forward and up are colinear
		static Quaternion lookRotation(const Vec3<T>& kForward, const Vec3<T>& kUp);

		// Input value in degree
		static Quaternion fromEuler(const Vec3<T>& kEuler);
		// Input values in degree
		static Quaternion fromEuler(T eulerX, T eulerY, T eulerZ);

		// Percent will be clamped to [0, 1]
		static Quaternion lerp(const Quaternion& start, const Quaternion& end, T percent);
		// Percent will be clamped to [0, 1]
		static Quaternion nlerp(const Quaternion& start, const Quaternion& end, T percent);
		// Input quaternions will be normalized, percent will be clamped to [0, 1]
		static Quaternion slerp(Quaternion start, Quaternion end, T percent);
		
		static const Quaternion skIdentity;

	public:
		LOAD_CONSTRUCTOR(Quaternion<T>)

		Quaternion();
		Quaternion(T w, T x, T y, T z);
		Quaternion(const Quaternion& kOther);
		// Input angle value in degree, axis will be normalized
		Quaternion(T angle, const Vec3<T>& kAxis);
		~Quaternion() = default;

		T modulus() const;
		T sqrModulus() const;

		Quaternion conjugated() const;
		// Returns identity if the modulus is null
		Quaternion inversed() const;
		// Returns identity if the modulus is null
		Quaternion normalized() const;

		void conjugate();
		// Does nothing if modulus is null
		void inverse();
		// Does nothing if modulus is null
		void normalize();

		T dot(const Quaternion& kOther) const;
		Quaternion clamp(const Quaternion& kMin, const Quaternion& kMax) const;

		std::string toString() const;
		// Returns euler in degree
		Vec3<T> toEuler() const;
		Vec3<T> toAxis() const;
		// Returns angle in degree
		T toAngle() const;

		Quaternion operator-() const;
		Quaternion operator*(T rhs) const;
		// Returns identity if the value is null
		Quaternion operator/(T rhs) const;
		Quaternion operator+(const Quaternion& kRhs) const;
		Quaternion operator-(const Quaternion& kRhs) const;
		Quaternion operator*(const Quaternion& kRhs) const;
		// Returns identity if the value is null
		Quaternion operator/(const Quaternion& kRhs) const;

		Vec3<T> operator*(const Vec3<T>& kRhs) const;

		Quaternion& operator=(const Quaternion& kRhs);
		Quaternion& operator*=(T rhs);
		// Does nothing f value is null
		Quaternion& operator/=(T rhs);
		Quaternion& operator+=(const Quaternion& kRhs);
		Quaternion& operator-=(const Quaternion& kRhs);
		Quaternion& operator*=(const Quaternion& kRhs);
		// Does nothing if value is null
		Quaternion& operator/=(const Quaternion& kRhs);

		bool operator==(const Quaternion& kRhs) const;
		bool operator!=(const Quaternion& kRhs) const;

		T mW;
		T mX;
		T mY;
		T mZ;
	};

	typedef Quaternion<float> Quatf;
}

META_CLASS(maj::math::Quatf, 4u)
META_FIELD(maj::math::Quatf, float, mW, 0u)
META_FIELD(maj::math::Quatf, float, mX, 1u)
META_FIELD(maj::math::Quatf, float, mY, 2u)
META_FIELD(maj::math::Quatf, float, mZ, 3u)

#include "Quaternion.inl"

#endif