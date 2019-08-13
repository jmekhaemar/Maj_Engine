#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <Service.h>
#include <ISerializable.h>

#if defined(_WIN32) && defined(MAJ_EDITOR)
#	ifdef Script_EXPORTS
#		define PROJECT_EXPORT __declspec(dllexport)
#	else
#		define PROJECT_EXPORT __declspec(dllimport)
#	endif
#	define PROJECT_EXTERN extern "C"
#else
#	define PROJECT_EXPORT
#	define PROJECT_EXTERN
#endif

#ifdef MAJ_EDITOR
#include <inspector/WInspectorComponent.h>

namespace maj
{
	PROJECT_EXTERN PROJECT_EXPORT const std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>& registeredClass();
	PROJECT_EXTERN PROJECT_EXPORT void services(const std::unordered_map<std::string, Service*>& kServices);

	namespace editor
	{
		PROJECT_EXTERN PROJECT_EXPORT const std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>>& registeredInspector();
		PROJECT_EXTERN PROJECT_EXPORT const std::unordered_map<uint32_t, WInspectorComponent::ComponentInfo>& registeredComponents();
	}
}
#else
#define REGISTER_INSPECTOR(CLASS)
#define REGISTER_INSPECTOR_CUSTOM(CLASS, NAME)
#define REGISTER_COMPONENT_INSPECTOR(CLASS)
#define REGISTER_COMPONENT_INSPECTOR_CUSTOM(CLASS, NAME)
#endif

#endif