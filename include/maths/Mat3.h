#ifndef _MAT3_H_
#define _MAT3_H_

#include <string>

#include "Vec3.h"
#include "Quaternion.h"

namespace maj::math
{
	template <typename T>
	class Mat3 final
	{
	public:
		// Input quaternion will be normalized
		static Mat3 makeRotation(const Quaternion<T>& kQuat);
		// Input value in degree
		static Mat3 makeRotation(const Vec3<T>& kEuler);
		
		// Input value in degree
		static Mat3 makeRotationX(T angle);
		// Input value in degree
		static Mat3 makeRotationY(T angle);
		// Input value in degree
		static Mat3 makeRotationZ(T angle);

		static const Mat3 skZero;
		static const Mat3 skIdentity;

	public:
		Mat3();
		Mat3(T e00, T e01, T e02, T e10, T e11, T e12, T e20, T e21, T e22);
		Mat3(const Mat3& kOther);

		T& at(uint8_t l, uint8_t c);
		T at(uint8_t l, uint8_t c) const;
		T& at(uint8_t index);
		T at(uint8_t index) const;
		T* elem();
		const T* elem() const;

		bool isOrtho() const;

		Mat3 transposed() const;
		// Returns identity if not ortho
		Mat3 inversed() const;

		void transpose();
		// Does nothing if not ortho
		void inverse();

		std::string toString() const;

		Mat3& operator-();
		Mat3 operator*(T rhs) const;
		// Returns identity if the value is null
		Mat3 operator/(T rhs) const;
		Mat3 operator*(const Mat3& kRhs) const;
		Vec3<T> operator*(const Vec3<T>& kRhs) const;
		
		Mat3& operator=(const Mat3& kRhs);
		Mat3& operator*=(T rhs);
		// Does noting if the value is null
		Mat3& operator/=(T rhs);
		Mat3& operator*=(const Mat3& kRhs);

		bool operator==(const Mat3& kRhs) const;
		bool operator!=(const Mat3& kRhs) const;
		
		// Returns mElem[0] if index is out of range
		T& operator[](uint8_t index);
		// Returns mElem[0] if index is out of range
		T operator[](uint8_t index) const;

	private:
		T mElem[9u];
	};

	typedef Mat3<float> Mat3f;
	typedef Mat3<double> Mat3d;
}

#include "Mat3.inl"

#endif