#include "ScriptModule.h"

#include <ISerializable.h>
#include <Service.h>
#include <Logging.h>

namespace maj
{
	ScriptModule::ScriptModule()
		: Service { typeid(ScriptModule).name() }, mHandle { nullptr }
	{}

	ScriptModule::~ScriptModule()
	{
		close();
	}

	void ScriptModule::load(const char* kName)
	{
		mOnLoad();
		close();
		open(kName);

		if (mHandle)
			mOnLoaded();
	}

	void ScriptModule::open(const char* kName)
	{
		// Loading the shared library
#ifdef _WIN32
		mHandle = LoadLibraryA(kName);
		if (!mHandle)
			THROW("failed to open script module: failed to load: " + std::string { kName })
#else
		mHandle = dlopen(kName, RTLD_LAZY);
		if (!mHandle)
			THROW("failed to open script module: failed to load: " + std::string { kName } +": " + std::string { dlerror() })
#endif

		// Getting registration
		typedef std::unordered_map<uint32_t, std::function<ISerializable* (JsonNode*)>> RegisterFuncMap;
		Function<const RegisterFuncMap&> registerFunc { function<const RegisterFuncMap&>("registeredClass") };
		if (!registerFunc)
			THROW("failed to open script module: failed to retrieve registration:" + std::string { kName })

		ISerializable::registerClass(registerFunc());

		// Getting services
		typedef std::unordered_map<std::string, Service*> ServiceMap;
		Function<void, const ServiceMap&> serviceFunc { function<void, const ServiceMap&>("services") };
		if (!serviceFunc)
			THROW("failed to open script module: failed to retrieve services:" + std::string { kName })
			
		serviceFunc(Service::services());
	}

	void ScriptModule::close()
	{
		if (!mHandle)
			return;

		mOnUnload();
		ISerializable::registeredClass().clear();

#ifdef _WIN32
		FreeLibrary(mHandle);
#else
		dlclose(mHandle);
#endif

		mHandle = nullptr;
	}
}