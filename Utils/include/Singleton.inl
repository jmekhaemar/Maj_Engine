#ifndef _SINGLETON_INL_
#define _SINGLETON_INL_

#include "Singleton.h"

namespace maj
{
	template<typename T>
	T Singleton<T>::sObj;

	template<typename T>
	constexpr T& Singleton<T>::instance()
	{
		return sObj;
	}
}

#endif