#ifndef _MAT4_H_
#define _MAT4_H_

#include "Quaternion.h"
#include "Vec4.h"

namespace maj::math
{
	template <typename T>
	class Mat4 final
	{
	public:
		static Mat4 makeTranslation(const Vec3<T>& kPos);

		// Input quaternion will be normalized
		static Mat4 makeRotation(const Quaternion<T>& kQuat);
		// Input value in degree
		static Mat4 makeRotation(const Vec3<T>& kEuler);

		// Input value in degree
		static Mat4 makeRotationX(T angle);
		// Input value in degree
		static Mat4 makeRotationY(T angle);
		// Input value in degree
		static Mat4 makeRotationZ(T angle);

		static Mat4 makeScale(const Vec3<T>& kScale); 

		static const Mat4 skZero;
		static const Mat4 skIdentity;

	public:
		Mat4();
		Mat4(T e00, T e01, T e02, T e03, T e10, T e11, T e12, T e13, T e20, T e21, T e22, T e23, T e30, T e31, T e32, T e33);
		Mat4(const Mat4& kOther);

		T& at(uint8_t l, uint8_t c);
		T at(uint8_t l, uint8_t c) const;
		T& at(uint8_t index);
		T at(uint8_t index) const;
		T* elem();
		const T* elem() const;

		bool isOrtho() const;
		bool isAffine() const;

		Mat4 transposed() const;
		// Returns identity if not ortho
		Mat4 inversed() const;
		
		void transpose();
		// Does nothing if not ortho
		void inverse();
		
		void applyTranslation(const Vec3<T>& kPos);
		// Input quaternion will be normalized
		void applyRotation(const Quaternion<T>& kQuat);
		void applyScale(const Vec3<T>& kScale);

		std::string toString() const;

		Mat4& operator-();
		Mat4 operator*(T rhs) const;
		// Returns identity if value is null
		Mat4 operator/(T rhs) const;
		Mat4 operator*(const Mat4& kRhs) const;
		Vec3<T> operator*(const Vec3<T>& kRhs) const;
		Vec4<T> operator*(const Vec4<T>& kRhs) const;

		Mat4& operator=(const Mat4& kRhs);
		Mat4& operator*=(T rhs);
		// Does nothing if value is null
		Mat4& operator/=(T rhs);
		Mat4& operator*=(const Mat4& kRhs);

		bool operator==(const Mat4& kRhs) const;
		bool operator!=(const Mat4& kRhs) const;

		// Returns mElem[0] if index is out of range
		T& operator[](uint8_t index);
		// Returns mElem[0] if index is out of range
		T operator[](uint8_t index) const;

	private:
		T mElem[16u];
	};

	typedef Mat4<float> Mat4f;
	typedef Mat4<double> Mat4d;
}

#include "Mat4.inl"

#endif