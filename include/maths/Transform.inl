#ifndef _TRANSFORM_INL_
#define _TRANSFORM_INL_

#include "Transform.h"

#include <Logging.h>

namespace maj::math
{
	template<typename T>
	DEFINE_HASH_OVERLOAD(Transform<T>)

	template<typename T>
	Transform<T> Transform<T>::lerp(const Transform<T>& kStart, const Transform<T>& kEnd, T percent)
	{
		percent = std::clamp(percent, static_cast<T>(0), static_cast<T>(1));

		return {
					Vec3<T>::lerp(kStart.mPos, kEnd.mPos, percent),
					Quaternion<T>::slerp(kStart.mRot, kEnd.mRot, percent),
					Vec3<T>::lerp(kStart.mScale, kEnd.mScale, percent)
				};
	}
	
	template<typename T>
	void Transform<T>::initialize() {}

	template<typename T>
	Transform<T>::Transform()
		: mPos {}, mRot {}, mScale { static_cast<T>(1) }
	{}

	template<typename T>
	Transform<T>::Transform(const Vec3<T>& kPos,const Quaternion<T>& kRot, const Vec3<T>& kScale)
		: mPos { kPos }, mRot { kRot }, mScale { kScale }
	{
		mRot.normalize();
	}

	template<typename T>
	Transform<T>::Transform(const Transform<T>& kOther)
		: mPos { kOther.mPos }, mRot { kOther.mRot }, mScale { kOther.mScale }
	{}

	template<typename T>
	Transform<T> Transform<T>::inversed() const
	{
		Quaternion<T> invRot { mRot.inversed() };
		invRot.normalize();

		Vec3<T> invScale { static_cast<T>(1) / mScale };
		Vec3<T> invPos { invRot * invScale.scaled(-mPos) };

		return { invPos, invRot, invScale };
	}

	template<typename T>
	void Transform<T>::inverse()
	{
		mRot.inverse();
		mRot.normalize();

		mScale = static_cast<T>(1) / mScale;
		mPos = mRot * mScale.scaled(-mPos);
	}

	template<typename T>
	const Vec3<T>& Transform<T>::position() const
	{
		return mPos;
	}

	template<typename T>
	const Quaternion<T>& Transform<T>::rotation() const
	{
		return mRot;
	}

	template<typename T>
	const Vec3<T>& Transform<T>::scale() const
	{
		return mScale;
	}

	template<typename T>
	Vec3<T> Transform<T>::forward() const
	{
		return mRot * Vec3<T>::skForward;
	}

	template<typename T>
	Vec3<T> Transform<T>::up() const
	{
		return mRot * Vec3<T>::skUp;
	}

	template<typename T>
	Vec3<T> Transform<T>::right() const
	{
		return mRot * Vec3<T>::skRight;
	}
	
	template<typename T>
	Mat4<T> Transform<T>::matrix() const
	{
		Mat4<T> mat{ Mat4<T>::makeTranslation(mPos) };
		mat.applyRotation(mRot);
		mat.applyScale(mScale);

		/*mat[12u] += mPos.mX;
		mat[13u] += mPos.mY;
		mat[14u] += mPos.mZ;*/

		return mat;
	}

	template<typename T>
	void Transform<T>::rotate(const Quaternion<T>& kRot)
	{
		mRot *= kRot;
		mRot.normalize();
	}

	template<typename T>
	void Transform<T>::translate(const Vec3<T>& kPos)
	{
		mPos += kPos;
	}

	template<typename T>
	void Transform<T>::lookAt(const Vec3<T>& kTarget, const Vec3<T>& kWorldUp)
	{
		mRot = math::Quatf::lookRotation(kTarget - mPos, kWorldUp);
	}

	template<typename T>
	void Transform<T>::rotateAround(const Vec3<T>& kPoint, T angle, const Vec3<T>& kAxis)
	{
		Vec3<T> dist { mPos - kPoint };
		Quaternion<T> quat { angle, kAxis };

		T length { dist.norm() };
		dist.normalize();
		quat.normalize();

		dist = quat * dist;
		mPos = kPoint + dist * length;
	}

	template<typename T>
	void Transform<T>::position(const Vec3<T>& kVal)
	{
		mPos = kVal;
	}

	template<typename T>
	void Transform<T>::rotation(const Quaternion<T>& kVal)
	{
		mRot = kVal;
		mRot.normalize();
	}

	template<typename T>
	void Transform<T>::scale(const Vec3<T>& kVal)
	{
		mScale = kVal;
	}

	template<typename T>
	std::string Transform<T>::toString() const
	{
		return "Transform(pos=" + mPos.toString() + ", rot=" + mRot.toString() + ", scale=" + mScale.toString() + ")";
	}

	template<typename T>
	Vec3<T> Transform<T>::operator*(const Vec3<T>& kRhs) const
	{
		return matrix() * kRhs;
	}

	template<typename T>
	Vec4<T> Transform<T>::operator*(const Vec4<T>& kRhs) const
	{
		return matrix() * kRhs;
	}

	template<typename T>
	Transform<T> Transform<T>::operator*(const Transform<T>& kRhs) const
	{
		Quaternion<T> rot { mRot * kRhs.mRot };
		rot.normalize();
		Vec3<T> scale { mScale.scaled(kRhs.mScale) };

		Vec3<T> pos { mPos + mRot * mScale.scaled(kRhs.mPos) };

		return { pos, rot, scale };
	}

	template<typename T>
	Transform<T>& Transform<T>::operator=(const Transform<T>& kRhs)
	{
		mPos = kRhs.mPos;
		mRot = kRhs.mRot;
		mScale = kRhs.mScale;

		return *this;
	}

	template<typename T>
	Transform<T>& Transform<T>::operator*=(const Transform<T>& kRhs)
	{
		mPos += mRot * mScale.scaled(kRhs.mPos);

		mRot *= kRhs.mRot;
		mRot.normalize();
		mScale = mScale.scaled(kRhs.mScale);

		return *this;
	}

	template<typename T>
	bool Transform<T>::operator==(const Transform<T>& kRhs) const
	{
		return mPos == kRhs.mPos && mRot == kRhs.mRot && mScale == kRhs.mScale;
	}

	template<typename T>
	bool Transform<T>::operator!=(const Transform<T>& kRhs) const
	{
		return mPos != kRhs.mPos || mRot != kRhs.mRot || mScale != kRhs.mScale;
	}
}

#endif
