#include "Service.h"

#include "Logging.h"

namespace maj
{
	std::unordered_map<std::string, Service*> Service::sServices;

	const std::unordered_map<std::string, Service*>& Service::services()
	{
		return sServices;
	}

	void Service::services(const std::unordered_map<std::string, Service*>& kServices)
	{
		for (const std::pair<const std::string, Service*>& kService : kServices)
			sServices.insert_or_assign(kService.first, kService.second);
	}

	bool Service::exist(const char* kName)
	{
		return sServices.find(kName) != sServices.end();
	}

	Service::Service(const char* kName)
		: mkName { kName }
	{
		if(!exist(kName))
			sServices[kName] = this;
		else
			LOG(LogWarning, "failed to create service: service \'" << kName << "\' already exists")
	}

	Service::~Service()
	{
		sServices.erase(mkName);
	}
}