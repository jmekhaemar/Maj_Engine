#ifndef _MAT3_INL_
#define _MAT3_INL_

#include "Mat3.h"

#include <algorithm>

#include <Logging.h>

#include "Helper.h"

namespace maj::math
{
	template<typename T>
	const Mat3<T> Mat3<T>::skZero
	{
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
	};

	template<typename T>
	const Mat3<T> Mat3<T>::skIdentity
	{
		static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(1),
	};

	template <typename T>
	Mat3<T> Mat3<T>::makeRotation(const Quaternion<T>& kQuat)
	{
		Quaternion<T> actual { kQuat.normalized() };
		T qx2 { actual.mX * actual.mX };
		T qy2 { actual.mY * actual.mY };
		T qz2 { actual.mZ * actual.mZ };

		Mat3<T> result {};
		result.mElem[0u] = static_cast<T>(1) - static_cast<T>(2) * ( qy2 + qz2 );
		result.mElem[3u] = static_cast<T>(2) * ( actual.mX * actual.mY - actual.mW * actual.mZ );
		result.mElem[6u] = static_cast<T>(2) * ( actual.mX * actual.mZ + actual.mW * actual.mY );

		result.mElem[1u] = static_cast<T>(2) * ( actual.mX * actual.mY + actual.mW * actual.mZ );
		result.mElem[4u] = static_cast<T>(1) - static_cast<T>(2) * ( qx2 + qz2 );
		result.mElem[7u] = static_cast<T>(2) * ( actual.mY * actual.mZ - actual.mW * actual.mX );

		result.mElem[2u] = static_cast<T>(2) * ( actual.mX * actual.mZ - actual.mW * actual.mY );
		result.mElem[5u] = static_cast<T>(2) * ( actual.mY * actual.mZ + actual.mW * actual.mX );
		result.mElem[8u] = static_cast<T>(1) - static_cast<T>(2) * ( qx2 + qy2 );

		return result;
	}

	template <typename T>
	Mat3<T> Mat3<T>::makeRotation(const Vec3<T>& kEuler)
	{
		T c1 { std::cos(kEuler.mX * DEG2RAD) };
		T s1 { std::sin(kEuler.mX * DEG2RAD) };
		T c2 { std::cos(kEuler.mY * DEG2RAD) };
		T s2 { std::sin(kEuler.mY * DEG2RAD) };
		T c3 { std::cos(kEuler.mZ * DEG2RAD) };
		T s3 { std::sin(kEuler.mZ * DEG2RAD) };

		return {	c2 * c3,	c1 * s3 + c3 * s1 * s2,		s1 * s3 - c1 * c3 * s2,
					-c2 * s3,	c1 * c3 - s1 * s2 * s3,		c3 * s1 + c1 * s2 * s3,
					s2,			-c2 * s1,					c1 * c2 };
	}

	template<typename T>
	Mat3<T> Mat3<T>::makeRotationX(T angle)
	{
		T cosA{ std::cos(angle * DEG2RAD) };
		T sinA{ std::sin(angle * DEG2RAD) };

		return { static_cast<T>(1),	static_cast<T>(0),	static_cast<T>(0),
				 static_cast<T>(0),	cosA,				sinA,
				 static_cast<T>(0),	-sinA,				cosA };
	}

	template<typename T>
	Mat3<T> Mat3<T>::makeRotationY(T angle)
	{
		T cosA{ std::cos(angle * DEG2RAD) };
		T sinA{ std::sin(angle * DEG2RAD) };

		return { cosA,				static_cast<T>(0),	-sinA,
				 static_cast<T>(0),	static_cast<T>(1),	static_cast<T>(0),
				 sinA,				static_cast<T>(0),	cosA };
	}

	template<typename T>
	Mat3<T> Mat3<T>::makeRotationZ(T angle)
	{
		T cosA{ std::cos(angle * DEG2RAD) };
		T sinA{ std::sin(angle * DEG2RAD) };

		return { cosA,				sinA,				static_cast<T>(0),
				-sinA,				cosA,				static_cast<T>(0),
				static_cast<T>(0),	static_cast<T>(0),	static_cast<T>(1) };
	}

	template<typename T>
	Mat3<T>::Mat3() :
		mElem { static_cast<T>(1),	static_cast<T>(0),	static_cast<T>(0),
				static_cast<T>(0),	static_cast<T>(1),	static_cast<T>(0),
				static_cast<T>(0),	static_cast<T>(0),	static_cast<T>(1) }
	{}

	template<typename T>
	Mat3<T>::Mat3(T e00, T e01, T e02, T e10, T e11, T e12, T e20, T e21, T e22) :
		mElem { e00, e01, e02,
				e10, e11, e12,
				e20, e21, e22}
	{}

	template<typename T>
	Mat3<T>::Mat3(const Mat3<T>& kOther)
		: mElem {	kOther.mElem[0u], kOther.mElem[1u], kOther.mElem[2u],
					kOther.mElem[3u], kOther.mElem[4u], kOther.mElem[5u], 
					kOther.mElem[6u], kOther.mElem[7u], kOther.mElem[8u] }
	{}

	template<typename T>
	T& Mat3<T>::at(uint8_t l, uint8_t c)
	{
		if (l >= 3u || c >= 3u)
		{
			LOG(LogWarning, "Mat3 at(x, y) : index out of array")
			return mElem[0u];
		}

		return mElem[l * 3u + c];
	}

	template<typename T>
	T Mat3<T>::at(uint8_t l, uint8_t c) const
	{
		if (l >= 3u || c >= 3u)
		{
			LOG(LogWarning, "Mat3 at(x, y) const : index out of array")
			return mElem[0u];
		}

		return mElem[l * 3u + c];
	}

	template<typename T>
	T& Mat3<T>::at(uint8_t index)
	{
		if (index >= 9u)
		{
			LOG(LogWarning, "Mat3 at(index) const : index out of array")
			return mElem[0u];
		}

		return mElem[index];
	}

	template<typename T>
	T Mat3<T>::at(uint8_t index) const
	{
		if (index >= 9u)
		{
			LOG(LogWarning, "Mat3 at(index) : index out of array")
			return mElem[0u];
		}

		return mElem[index];
	}

	template<typename T>
	T* Mat3<T>::elem()
	{
		return mElem;
	}

	template<typename T>
	const T* Mat3<T>::elem() const
	{
		return mElem;
	}

	template<typename T>
	bool  Mat3<T>::isOrtho() const
	{
		Vec3<T> l1{ mElem[0u], mElem[1u], mElem[2u] };
		Vec3<T> l2{ mElem[3u], mElem[4u], mElem[5u] };
		Vec3<T> l3{ mElem[6u], mElem[7u], mElem[8u] };

		return Helper<T>::areSame(l1.sqrNorm(), static_cast<T>(1)) && Helper<T>::areSame(l2.sqrNorm(), static_cast<T>(1)) && Helper<T>::areSame(l3.sqrNorm(), static_cast<T>(1)) &&
			   Helper<T>::areSame(l1.dot(l2), static_cast<T>(0)) && Helper<T>::areSame(l1.dot(l3), static_cast<T>(0)) && Helper<T>::areSame(l2.dot(l3), static_cast<T>(0));
	}

	template<typename T>
	Mat3<T> Mat3<T>::transposed() const
	{
		return { mElem[0u], mElem[3u], mElem[6u],
				 mElem[1u], mElem[4u], mElem[7u],
				 mElem[2u], mElem[5u], mElem[8u] };
	}

	template<typename T>
	Mat3<T> Mat3<T>::inversed() const
	{
		if (!isOrtho())
		{
			LOG(LogWarning, "Mat3 inversed : matrix not ortho")
			return {};
		}

		return transposed();
	}

	template<typename T>
	void Mat3<T>::transpose()
	{
		for (uint8_t y { 0u }; y < 3u; y++)
			for (uint8_t x { y }; x < 3u; x++)
				std::swap(mElem[y * 3u + x], mElem[x * 3u + y]);
	}

	template<typename T>
	void Mat3<T>::inverse()
	{
		if (!isOrtho())
		{
			LOG(LogWarning, "Mat3 inverse : matrix not ortho");
			return;
		}

		transpose();
	}

	template<typename T>
	std::string Mat3<T>::toString() const
	{
		std::string str{ "Mat3(\n" };

		for (uint8_t l { 0u }; l < 3u; l++)
		{
			for (uint8_t c { 0u }; c < 3u; c++)
				str = str + std::to_string(at(l, c)) + "\t";

			str = str + "\n";
		}

		str = str + ")";
		return str;
	}

	template<typename T>
	Mat3<T>& Mat3<T>::operator-()
	{
		for (uint8_t i { 0u }; i < 9u; i++)
			mElem[i] = -mElem[i];

		return *this;
	}

	template<typename T>
	Mat3<T> Mat3<T>::operator*(T rhs) const
	{
		Mat3<T> result { skZero };
		for (uint8_t i { 0u }; i < 9u; i++)
			result[i] = mElem[i] * rhs;

		return result;
	}

	template<typename T>
	Mat3<T> Mat3<T>::operator/(T rhs) const
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Mat3 operator/ : division by 0")
			return {};
		}

		T inv { static_cast<T>(1) / rhs };

		Mat3<T> result { skZero };
		for (uint8_t i { 0u }; i < 9u; i++)
			result[i] = mElem[i] * inv;

		return result;
	}

	template<typename T>
	Mat3<T> Mat3<T>::operator*(const Mat3<T>& kRhs) const
	{
		Mat3<T> res { skZero };

		for (uint8_t i { 0u }; i < 3u; i++)
		{
			for (uint8_t j { 0u }; j < 3u; j++)
			{
				for (uint8_t k { 0u }; k < 3u; k++)
					res.mElem[i * 3u + j] += mElem[k * 3u + j] * kRhs.mElem[i * 3u + k];
			}
		}

		return res;
	}

	template<typename T>
	Vec3<T> Mat3<T>::operator*(const Vec3<T>& kRhs) const
	{
		return { mElem[0u] * kRhs.mX + mElem[3u] * kRhs.mY + mElem[6u] * kRhs.mZ,
				 mElem[1u] * kRhs.mX + mElem[4u] * kRhs.mY + mElem[7u] * kRhs.mZ,
				 mElem[2u] * kRhs.mX + mElem[5u] * kRhs.mY + mElem[8u] * kRhs.mZ };
	}

	template<typename T>
	Mat3<T>& Mat3<T>::operator=(const Mat3<T>& kRhs)
	{
		std::memcpy(mElem, kRhs.mElem, sizeof(T) * 9u);

		return *this;
	}

	template<typename T>
	Mat3<T>& Mat3<T>::operator*=(T rhs)
	{
		for (uint8_t i { 0u }; i < 9u; i++)
			mElem[i] *= rhs;

		return *this;
	}

	template<typename T>
	Mat3<T>& Mat3<T>::operator/=(T rhs)
	{
		if (Helper<T>::areSame(rhs, static_cast<T>(0)))
		{
			LOG(LogWarning, "Mat3 operator/ : division by 0")
			return *this;
		}

		T inv { static_cast<T>(1) / rhs };
		for (uint8_t i { 0u }; i < 9u; i++)
			mElem[i] *= inv;

		return *this;
	}

	template<typename T>
	Mat3<T>& Mat3<T>::operator*=(const Mat3<T>& kRhs)
	{
		// Avoid collision
		if (mElem == kRhs.mElem)
			return operator=(operator*(kRhs));

		Mat3<T> temp { *this };

		for (uint8_t i { 0u }; i < 3u; i++)
		{
			for (uint8_t j { 0u }; j < 3u; j++)
			{
				mElem[i * 3u + j] = temp.mElem[j] * kRhs.mElem[i * 3u]
									+ temp.mElem[3u + j] * kRhs.mElem[i * 3u + 1u]
									+ temp.mElem[6u + j] * kRhs.mElem[i * 3u + 2u];
			}
		}

		return *this;
	}

	template<typename T>
	bool Mat3<T>::operator==(const Mat3<T>& kRhs) const
	{
		for (uint8_t i { 0u }; i < 9u; i++)
			if (!Helper<T>::areSame(mElem[i], kRhs.mElem[i]))
				return false;

		return true;
	}

	template<typename T>
	bool Mat3<T>::operator!=(const Mat3<T>& rhs) const
	{
		for (uint8_t i { 0u }; i < 9u; i++)
			if (!Helper<T>::areSame(mElem[i], rhs.mElem[i]))
				return true;

		return false;
	}

	template<typename T>
	T& Mat3<T>::operator[](uint8_t index)
	{
		if(index < 9u)
			return mElem[index];
		
		LOG(LogWarning, "Mat3 operator[index] : index out of range")
		return mElem[0u];
	}

	template<typename T>
	T Mat3<T>::operator[](uint8_t index) const
	{
		if (index < 9u)
			return mElem[index];

		LOG(LogWarning, "Mat3 operator[index] const : index out of range")
		return mElem[0u];
	}
}

#endif
