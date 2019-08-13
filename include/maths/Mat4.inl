#ifndef _MAT4_INL_
#define _MAT4_INL_

#include "Mat4.h"

#include <cmath>

#include <Logging.h>

#include "Helper.h"

namespace maj::math
{
	template<typename T>
	const Mat4<T> Mat4<T>::skZero
	{
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)
	};

	template<typename T>
	const Mat4<T> Mat4<T>::skIdentity
	{
		static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)
	};

	template<typename T>
	Mat4<T> Mat4<T>::makeTranslation(const Vec3<T>& kPos)
	{
		Mat4<T> matrix;

		matrix.mElem[12u] = kPos.mX;
		matrix.mElem[13u] = kPos.mY;
		matrix.mElem[14u] = kPos.mZ;

		return matrix;
	}

	template<typename T>
	Mat4<T> Mat4<T>::makeRotation(const Quaternion<T>& kQuat)
	{
		Quaternion<T> actual{ kQuat.normalized() };
		T qx2{ actual.mX * actual.mX };
		T qy2{ actual.mY * actual.mY };
		T qz2{ actual.mZ * actual.mZ };

		Mat4<T> result {};
		result.mElem[0u] = static_cast<T>(1) - static_cast<T>(2) * (qy2 + qz2);
		result.mElem[4u] = static_cast<T>(2) * (actual.mX * actual.mY - actual.mW * actual.mZ);
		result.mElem[8u] = static_cast<T>(2) * (actual.mX * actual.mZ + actual.mW * actual.mY);

		result.mElem[1u] = static_cast<T>(2) * (actual.mX * actual.mY + actual.mW * actual.mZ);
		result.mElem[5u] = static_cast<T>(1) - static_cast<T>(2) * ( qx2 + qz2 );
		result.mElem[9u] = static_cast<T>(2) * (actual.mY * actual.mZ - actual.mW * actual.mX);

		result.mElem[2u] = static_cast<T>(2) * (actual.mX * actual.mZ - actual.mW * actual.mY);
		result.mElem[6u] = static_cast<T>(2) * (actual.mY * actual.mZ + actual.mW * actual.mX);
		result.mElem[10u] = static_cast<T>(1) - static_cast<T>(2) * (qx2 + qy2);

		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::makeRotation(const Vec3<T>& kEuler)
	{
		T c1 { std::cos(kEuler.mX * DEG2RAD) };
		T s1 { std::sin(kEuler.mX * DEG2RAD) };
		T c2 { std::cos(kEuler.mY * DEG2RAD) };
		T s2 { std::sin(kEuler.mY * DEG2RAD) };
		T c3 { std::cos(kEuler.mZ * DEG2RAD) };
		T s3 { std::sin(kEuler.mZ * DEG2RAD) };

		return { c2 * c3,				c1 * s3 + c3 * s1 * s2,		s1 * s3 - c1 * c3 * s2,	static_cast<T>(0),
				-c2 * s3,				c1 * c3 - s1 * s2 * s3,		c3 * s1 + c1 * s2 * s3,	static_cast<T>(0),
				s2,						-c2 * s1,					c1 * c2,				static_cast<T>(0),
				static_cast<T>(0),		static_cast<T>(0),			static_cast<T>(0),		static_cast<T>(1) };
	}

	template<typename T>
	Mat4<T> Mat4<T>::makeRotationX(T angle)
	{
		T cosA { std::cos(angle * DEG2RAD) };
		T sinA { std::sin(angle* DEG2RAD) };

		return {static_cast<T>(1), static_cast<T>(0),	static_cast<T>(0),	static_cast<T>(0),
				static_cast<T>(0), cosA,				sinA,				static_cast<T>(0),
				static_cast<T>(0), -sinA,				cosA,				static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0),	static_cast<T>(0),	static_cast<T>(1) };
	}

	template<typename T>
	Mat4<T> Mat4<T>::makeRotationY(T angle)
	{
		T cosA { std::cos(angle * DEG2RAD) };
		T sinA { std::sin(angle* DEG2RAD) };

		return {cosA,				static_cast<T>(0), -sinA,				static_cast<T>(0),
				static_cast<T>(0),	static_cast<T>(1), static_cast<T>(0),	static_cast<T>(0),
				sinA,				static_cast<T>(0), cosA,				static_cast<T>(0),
				static_cast<T>(0),	static_cast<T>(0), static_cast<T>(0),	static_cast<T>(1) };
	}

	template<typename T>
	Mat4<T> Mat4<T>::makeRotationZ(T angle)
	{
		T cosA { std::cos(angle * DEG2RAD) };
		T sinA { std::sin(angle* DEG2RAD) };

		return {cosA,				sinA,				static_cast<T>(0),	static_cast<T>(0),
				-sinA,				cosA,				static_cast<T>(0),	static_cast<T>(0),
				static_cast<T>(0),	static_cast<T>(0),	static_cast<T>(1),	static_cast<T>(0),
				static_cast<T>(0),	static_cast<T>(0),	static_cast<T>(0),	static_cast<T>(1) };
	}

	template<typename T>
	Mat4<T> Mat4<T>::makeScale(const Vec3<T>& kScale)
	{
		Mat4<T> matrix;

		for (uint8_t i { 0u }; i < 3u; i++)
			matrix.mElem[i * 4u + i] = kScale[i];

		return matrix;
	}

	template<typename T>
	Mat4<T>::Mat4()
		: mElem {	static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
					static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
					static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
					static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) }
	{}

	template<typename T>
	Mat4<T>::Mat4(T e00, T e01, T e02, T e03, T e10, T e11, T e12, T e13, T e20, T e21, T e22, T e23, T e30, T e31, T e32, T e33):
		mElem { e00, e01, e02, e03,
				e10, e11, e12, e13,
				e20, e21, e22, e23,
				e30, e31, e32, e33 }
	{}

	template<typename T>
	Mat4<T>::Mat4(const Mat4<T>& kOther)
		: mElem {	kOther.mElem[0u], kOther.mElem[1u], kOther.mElem[2u], kOther.mElem[3u],
					kOther.mElem[4u], kOther.mElem[5u], kOther.mElem[6u], kOther.mElem[7u],
					kOther.mElem[8u], kOther.mElem[9u], kOther.mElem[10u], kOther.mElem[11u],
					kOther.mElem[12u], kOther.mElem[13u], kOther.mElem[14u], kOther.mElem[15u] }
	{}

	template<typename T>
	T& Mat4<T>::at(uint8_t l, uint8_t c)
	{
		if (l >= 4u || c >= 4u)
		{
			LOG(LogWarning, "Mat4 at(x, y) : index out of array")
			return mElem[0u];
		}

		return mElem[l * 4u + c];
	}

	template<typename T>
	T Mat4<T>::at(uint8_t l, uint8_t c) const
	{
		if (l >= 4u || c >= 4u)
		{
			LOG(LogWarning, "Mat4 at(x, y) const : index out of array")
			return mElem[0u];
		}

		return mElem[l * 4u + c];
	}

	template<typename T>
	T& Mat4<T>::at(uint8_t index)
	{
		if (index >= 16u)
		{
			LOG(LogWarning, "Mat4 at(index) : index out of array")
			return mElem[0u];
		}

		return mElem[index];
	}

	template<typename T>
	T Mat4<T>::at(uint8_t index) const
	{
		if (index >= 16u)
		{
			LOG(LogWarning, "Mat4 at(index) const : index out of array")
			return mElem[0u];
		}

		return mElem[index];
	}

	template<typename T>
	T* Mat4<T>::elem()
	{
		return mElem;
	}

	template<typename T>
	const T* Mat4<T>::elem() const
	{
		return mElem;
	}

	template<typename T>
	bool Mat4<T>::isOrtho() const 
	{
		Vec4<T> l1{ mElem[0u], mElem[1u], mElem[2u], mElem[3u] };
		Vec4<T> l2{ mElem[4u], mElem[5u], mElem[6u], mElem[7u] };
		Vec4<T> l3{ mElem[8u], mElem[9u], mElem[10u], mElem[11u] };
		Vec4<T> l4{ mElem[12u], mElem[13u], mElem[14u], mElem[15u] };

		return	Helper<T>::areSame(l1.sqrNorm(), static_cast<T>(1)) && Helper<T>::areSame(l2.sqrNorm(), static_cast<T>(1)) && Helper<T>::areSame(l3.sqrNorm(), static_cast<T>(1)) && 
				Helper<T>::areSame(l4.sqrNorm(), static_cast<T>(1)) && Helper<T>::areSame(l1.dot(l2), static_cast<T>(0)) && Helper<T>::areSame(l1.dot(l3), static_cast<T>(0)) && 
				Helper<T>::areSame(l1.dot(l4), static_cast<T>(0)) && Helper<T>::areSame(l2.dot(l3), static_cast<T>(0)) && Helper<T>::areSame(l2.dot(l4), static_cast<T>(0)) && 
				Helper<T>::areSame(l3.dot(l4), static_cast<T>(0));
	}

	template<typename T>
	bool Mat4<T>::isAffine() const
	{
		return  Helper<T>::areSame(mElem[3u], static_cast<T>(0)) && Helper<T>::areSame(mElem[7u], static_cast<T>(0)) && Helper<T>::areSame(mElem[11u], static_cast<T>(0)) &&
				Helper<T>::areSame(mElem[0u], mElem[5u]) && Helper<T>::areSame(mElem[0u], mElem[10u]) && Helper<T>::areSame(mElem[5u], mElem[10u]);
	}

	template<typename T>
	Mat4<T> Mat4<T>::transposed() const
	{
		return { mElem[0u], mElem[4u], mElem[8u], mElem[12u],
				 mElem[1u], mElem[5u], mElem[9u], mElem[13u],
				 mElem[2u], mElem[6u], mElem[10u], mElem[14u],
				 mElem[3u], mElem[7u], mElem[11u], mElem[15u] };
	}

	template<typename T>
	Mat4<T> Mat4<T>::inversed() const
	{
		if (!isOrtho())
		{
			LOG(LogWarning, "Mat4 inversed : matrix not ortho")
			return {};
		}

		return transposed();
	}

	template<typename T>
	void Mat4<T>::transpose()
	{
		for (uint8_t y { 0u }; y < 4u; y++)
			for (uint8_t x =  y + 1u ; x < 4u; x++)
				std::swap(mElem[y * 4u + x], mElem[x * 4u + y]);
	}

	template<typename T>
	void Mat4<T>::inverse()
	{
		if (!isOrtho())
		{
			LOG(LogWarning, "Mat4 inverse : matrix not ortho");
			return;
		}
		
		transpose();
	}

	template<typename T>
	void Mat4<T>::applyTranslation(const Vec3<T>& kPos)
	{
		mElem[12u] += mElem[0u] * kPos.mX + mElem[4u] * kPos.mY + mElem[8u] * kPos.mZ;
		mElem[13u] += mElem[1u] * kPos.mX + mElem[5u] * kPos.mY + mElem[9u] * kPos.mZ;
		mElem[14u] += mElem[2u] * kPos.mX + mElem[6u] * kPos.mY + mElem[10u] * kPos.mZ;
		mElem[15u] += mElem[3u] * kPos.mX + mElem[7u] * kPos.mY + mElem[11u] * kPos.mZ;
	}

	template<typename T>
	void Mat4<T>::applyRotation(const Quaternion<T>& kQuat)
	{
		Mat4<T> rot { Mat4::makeRotation(kQuat) };

		// Pre multiplication rot * temp, knowing rot is a mat3
		Mat4<T> temp { *this };
		for (uint8_t i { 0u }; i < 3u; i++)
		{
			for (uint8_t j { 0u }; j < 4u; j++)
			{
				mElem[i * 4u + j] = temp.mElem[j] * rot.mElem[i * 4u]
									+ temp.mElem[4u + j] * rot.mElem[i * 4u + 1u]
									+ temp.mElem[8u + j] * rot.mElem[i * 4u + 2u];

				if (j != 3u)
					mElem[i * 4u + j] += temp.mElem[12u + j] * rot.mElem[i * 4u + 3u];
			}
		}
	}

	template<typename T>
	void  Mat4<T>::applyScale(const Vec3<T>& kScale)
	{
		mElem[0u] *= kScale.mX;
		mElem[1u] *= kScale.mX;
		mElem[2u] *= kScale.mX;
		mElem[3u] *= kScale.mX;

		mElem[4u] *= kScale.mY;
		mElem[5u] *= kScale.mY;
		mElem[6u] *= kScale.mY;
		mElem[7u] *= kScale.mY;

		mElem[8u] *= kScale.mZ;
		mElem[9u] *= kScale.mZ;
		mElem[10u] *= kScale.mZ;
		mElem[11u] *= kScale.mZ;
	}

	template<typename T>
	std::string Mat4<T>::toString() const
	{
		std::string str{ "Mat4(\n" };

		for (uint8_t l { 0u }; l < 4u; l++)
		{
			for (uint8_t c { 0u }; c < 4u; c++)
				str = str + std::to_string(mElem[l * 4u + c]) + "\t";

			str = str + "\n";
		}

		str = str + ")";
		return str;
	}

	template<typename T>
	Mat4<T>& Mat4<T>::operator-()
	{
		for (uint8_t i { 0u }; i < 16u; i++)
			mElem[i] = -mElem[i];

		return *this;
	}

	template<typename T>
	Mat4<T> Mat4<T>::operator*(T rhs) const
	{
		Mat4<T> result { skZero };
		for (uint8_t i { 0u }; i < 16u; i++)
			result[i] = mElem[i] * rhs;

		return result;
	}

	template<typename T>
	Mat4<T> Mat4<T>::operator/(T rhs) const
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Mat4 operator/ : division by 0")
			return {};
		}

		T inv { static_cast<T>(1) / rhs };
		Mat4<T> result { skZero };

		for (uint8_t i { 0u }; i < 16u; i++)
			result[i] = mElem[i] * inv;

		return result;
	}

	template<typename T>
	Mat4<T> Mat4<T>::operator*(const Mat4<T>& kRhs) const
	{
		Mat4<T> res{skZero};
		for (uint8_t i { 0u }; i < 4u; i++)
		{
			for (uint8_t j { 0u }; j < 4u; j++)
			{
				for (uint8_t k { 0u }; k < 4u; k++)
					res.mElem[i * 4u + j] += mElem[k * 4u + j] * kRhs.mElem[i * 4u + k];
			}
		}

		return res;
	}

	template<typename T>
	Vec3<T> Mat4<T>::operator*(const Vec3<T>& kRhs) const
	{
		return {	mElem[0u] * kRhs.mX + mElem[4u] * kRhs.mY + mElem[8u] * kRhs.mZ + mElem[12u],
					mElem[1u] * kRhs.mX + mElem[5u] * kRhs.mY + mElem[9u] * kRhs.mZ + mElem[13u],
					mElem[2u] * kRhs.mX + mElem[6u] * kRhs.mY + mElem[10u] * kRhs.mZ + mElem[14u] };
	}

	template<typename T>
	Vec4<T> Mat4<T>::operator*(const Vec4<T>& kRhs) const
	{
		return {	mElem[0u] * kRhs.mX + mElem[4u] * kRhs.mY + mElem[8u] * kRhs.mZ + mElem[12u] * kRhs.mW,
					mElem[1u] * kRhs.mX + mElem[5u] * kRhs.mY + mElem[9u] * kRhs.mZ + mElem[13u] * kRhs.mW,
					mElem[2u] * kRhs.mX + mElem[6u] * kRhs.mY + mElem[10u] * kRhs.mZ + mElem[14u] * kRhs.mW,
					mElem[3u] * kRhs.mX + mElem[7u] * kRhs.mY + mElem[11u] * kRhs.mZ + mElem[15u] * kRhs.mW	};
	}

	template<typename T>
	Mat4<T>& Mat4<T>::operator=(const Mat4<T>& kRhs)
	{
		if (mElem == kRhs.mElem)
			return *this;

		std::memcpy(mElem, kRhs.mElem, sizeof(T) * 16u);

		return *this;
	}

	template<typename T>
	Mat4<T>& Mat4<T>::operator*=(T rhs)
	{
		for (uint8_t i { 0u }; i < 16u; i++)
			mElem[i] *= rhs;

		return *this;
	}

	template<typename T>
	Mat4<T>& Mat4<T>::operator/=(T rhs)
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Mat4 operator/= : division by 0")
			return *this;
		}

		T inv { static_cast<T>(1) / rhs };
		for (uint8_t i { 0u }; i < 16u; i++)
			mElem[i] *= inv;

		return *this;
	}

	template<typename T>
	Mat4<T>& Mat4<T>::operator*=(const Mat4<T>& kRhs)
	{
		// Avoid collision
		if (mElem == kRhs.mElem)
			return operator=(operator*(kRhs));

		Mat4<T> temp { *this };

		for (uint8_t i { 0u }; i < 4u; i++)
		{
			for (uint8_t j { 0u }; j < 4u; j++)
			{
				mElem[i * 4u + j] = temp.mElem[j] * kRhs.mElem[i * 4u]
									+ temp.mElem[4u + j] * kRhs.mElem[i * 4u + 1u]
									+ temp.mElem[8u + j] * kRhs.mElem[i * 4u + 2u]
									+ temp.mElem[12u + j] * kRhs.mElem[i * 4u + 3u];
			}
		}

		return *this;
	}

	template<typename T>
	bool Mat4<T>::operator==(const Mat4<T>& kRhs) const
	{
		return !(operator!=(kRhs));
	}

	template<typename T>
	bool Mat4<T>::operator!=(const Mat4<T>& kRhs) const
	{
		for (uint8_t i { 0u }; i < 16u; i++)
			if (!Helper<T>::areSame(mElem[i], kRhs.mElem[i]))
				return true;

		return false;
	}

	template<typename T>
	T& Mat4<T>::operator[](uint8_t index)
	{
		if (index >= 16u)
		{
			LOG(LogWarning, "Mat4 operator[] : index out of array")
			return mElem[0u];
		}

		return mElem[index];
	}

	template<typename T>
	T Mat4<T>::operator[](uint8_t index) const
	{
		if (index >= 16u)
		{
			LOG(LogWarning, "Mat4 operator[] const : index out of array")
			return mElem[0u];
		}

		return mElem[index];
	}
}

#endif
