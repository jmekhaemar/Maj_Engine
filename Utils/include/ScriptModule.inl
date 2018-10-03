#ifndef _SCRIPT_MODULE_INL_
#define _SCRIPT_MODULE_INL_

#include "ScriptModule.h"

namespace maj
{
	template <typename R, typename... Args>
	ScriptModule::Function<R, Args...> ScriptModule::function(const char* kName)
	{
		Function<R, Args...> function { nullptr };
		if (!mHandle)
			return function;

#ifdef _WIN32
		function = reinterpret_cast<Function<R, Args...>>(GetProcAddress(mHandle, kName));
#else
		function = reinterpret_cast<Function<R, Args...>>(dlsym(mHandle, kName));
#endif

		return function;
	}
}

#endif