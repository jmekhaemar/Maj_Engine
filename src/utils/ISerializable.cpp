#include "ISerializable.h"

#include <cstdlib>

namespace maj
{
	void ISerializable::registerClass(const std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>& kClasses)
	{
		std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>& registered { registeredClass() };

		// Remove unused symbols
		for (std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>::iterator it { registered.begin() } ; it != registered.end() ;)
		{
			if (kClasses.count(it->first) == 0u)
				it = registered.erase(it);
			else
				++it;
		}

		// Add used symbols
		for (const std::pair<const uint32_t, std::function<ISerializable*(JsonNode*)>>& kClass : kClasses)
		{
			if (registered.count(kClass.first) != 0u)
				continue;

			registered.emplace(kClass.first, kClass.second);
		}
	}

	ISerializable::ISerializable()
		: mGuid { static_cast<uint32_t>(std::rand()) }
	{}
	
	ISerializable::ISerializable(const ISerializable& kOther)
		: mGuid { kOther.mGuid }
	{}

	void ISerializable::initialize()
	{}
}