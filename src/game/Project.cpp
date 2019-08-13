#include "Project.h"

#ifdef MAJ_EDITOR
namespace maj
{
	PROJECT_EXTERN PROJECT_EXPORT const std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>& registeredClass()
	{
		return ISerializable::registeredClass();
	}

	PROJECT_EXTERN PROJECT_EXPORT void services(const std::unordered_map<std::string, Service*>& kServices)
	{
		return Service::services(kServices);
	}

	namespace editor
	{
		PROJECT_EXTERN PROJECT_EXPORT const std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>>& registeredInspector()
		{
			return WInspectorComponent::registeredInspector();
		}

		PROJECT_EXTERN PROJECT_EXPORT const std::unordered_map<uint32_t, WInspectorComponent::ComponentInfo>& registeredComponents()
		{
			return WInspectorComponent::registeredComponents();
		}
	}
}
#endif