#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Mat4.h"

namespace maj::math
{
	template<typename T>
	class Transform final : public ISerializable
	{
		ENABLE_META(Transform<T>)

	public:
		// Percent will be clamped to [0, 1]
		static Transform lerp(const Transform& kStart, const Transform& kEnd, T percent);

	public:
		LOAD_CONSTRUCTOR(Transform<T>)

		Transform();
		Transform(const Vec3<T>& kPos, const Quaternion<T>& kRot, const Vec3<T>& kScale = { static_cast<T>(1) });
		Transform(const Transform& kOther);

		// Result rotation may not be normalized
		Transform inversed() const;
		// Result rotation may not be normalized
		void inverse();

		const Vec3<T>& position() const;
		// Rotation may not be normalized
		const Quaternion<T>& rotation() const;
		const Vec3<T>& scale() const;

		Vec3<T> forward() const;
		Vec3<T> up() const;
		Vec3<T> right() const;
		Mat4<T> matrix() const;

		// Result rotation may not be normalized
		void rotate(const Quaternion<T>& kRot);
		void translate(const Vec3<T>& kPos);

		// Result rotation may not be normalized
		void lookAt(const Vec3<T>& kTarget, const Vec3<T>& kWorldUp = Vec3<T>::skUp);
		void rotateAround(const Vec3<T>& kPoint, T angle, const Vec3<T>& kAxis = Vec3<T>::skUp);

		void position(const Vec3<T>& kVal);
		// Result rotation may not be normalized
		void rotation(const Quaternion<T>& kVal);
		void scale(const Vec3<T>& kVal);

		std::string toString() const;

		Vec3<T> operator*(const Vec3<T>& kRhs) const;
		Vec4<T> operator*(const Vec4<T>& kRhs) const;
		Transform operator*(const Transform& kRhs) const;

		Transform& operator=(const Transform& kRhs);
		Transform& operator*=(const Transform& kRhs);

		bool operator==(const Transform& kRhs) const;
		bool operator!=(const Transform& kRhs) const;
		
	private:
		Vec3<T> mPos;
		Quaternion<T> mRot;
		Vec3<T> mScale;
	};

	typedef Transform<float> Transf;
}

META_CLASS(maj::math::Transf, 3u)
META_FIELD(maj::math::Transf, maj::math::Vec3f, mPos, 0u)
META_FIELD(maj::math::Transf, maj::math::Quatf, mRot, 1u)
META_FIELD(maj::math::Transf, maj::math::Vec3f, mScale, 2u)

#include "Transform.inl"

#endif