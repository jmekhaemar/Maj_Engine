#ifndef _WINSPECTOR_ASSET_H_
#define _WINSPECTOR_ASSET_H_

#include "WInspectorComponent.h"

namespace maj::editor
{
	class WInspectorAsset : public WInspectorComponent
	{
	public:
		inline static std::unordered_map<uint32_t, std::function<WInspectorAsset*(uint32_t, QWidget*, QFormLayout*)>>& registeredAssetInspector();

	public:
		WInspectorAsset(QWidget* parent, QFormLayout* layout = nullptr, std::function<void()> callback = nullptr);

		template<typename T>
		void load(T* component);
	};

	template<typename T, typename Inspector = WInspectorAsset>
	struct RegisterAssetInspector final
	{
		RegisterAssetInspector();
	};
}

#include "WInspectorAsset.inl"

#define REGISTER_ASSET_INSPECTOR(CLASS) maj::editor::RegisterAssetInspector<CLASS> __ ## CLASS;
#define REGISTER_ASSET_INSPECTOR_CUSTOM(CLASS, NAME) maj::editor::RegisterAssetInspector<CLASS, NAME> __ ## CLASS;

#endif