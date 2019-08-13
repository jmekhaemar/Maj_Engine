#ifndef _MATH_HELPER_INL_
#define _MATH_HELPER_INL_

#include "Helper.h"

#include <cmath>

namespace maj::math
{
	template<typename T>
	const T Helper<T>::skEpsilon{ static_cast<T>(1E-5) };

	template<typename T>
	T Helper<T>::lerp(T start, T end, T alpha)
	{
		return (static_cast<T>(1) - alpha) * start + alpha * end;
	}
	
	template<typename T>
	bool Helper<T>::areSame(T a, T b)
	{
		return std::fabs(a - b) < skEpsilon;
	}
}

#endif