#ifndef _SCRIPT_MODULE_H_
#define _SCRIPT_MODULE_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <Service.h>
#include <Event.h>

namespace maj
{
	class ScriptModule final : public Service
	{
	public:
		template <typename R, typename... Args>
		using Function = R(*)(Args...);

		ScriptModule();
		~ScriptModule();

		void load(const char* kName);

		template <typename R, typename... Args>
		Function<R, Args...> function(const char* kName);

		Event<void> mOnLoad;
		Event<void> mOnUnload;
		Event<void> mOnLoaded;

	private:
		void open(const char* kName);
		void close();

#ifdef _WIN32
		HINSTANCE mHandle;
#else
		void* mHandle;
#endif
	};
}

#include "ScriptModule.inl"

#endif