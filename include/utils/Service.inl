#ifndef _SERVICE_INL_
#define _SERVICE_INL_

#include "Service.h"

namespace maj
{
	template<typename T>
	T* Service::get()
	{
		const char* kName { typeid(T).name() };
		if (exist(kName))
			return reinterpret_cast<T*>(sServices.at(kName));

		return nullptr;
	}
}

#endif