#ifndef _WINSPECTOR_ASSET_INL_
#define _WINSPECTOR_ASSET_INL_

#include "WInspectorAsset.h"

#include <qpushbutton.h>

#include "Engine.h"

#include <Image.h>

namespace maj::editor
{
	std::unordered_map<uint32_t, std::function<WInspectorAsset*(uint32_t, QWidget*, QFormLayout*)>>& WInspectorAsset::registeredAssetInspector()
	{
		static std::unordered_map<uint32_t, std::function<WInspectorAsset*(uint32_t, QWidget*, QFormLayout*)>> sRegisteredAssetInspector{};
		return sRegisteredAssetInspector;
	}

	template<typename T>
	void WInspectorAsset::load(T* component)
	{
		if (!component)
			return;

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		if constexpr (meta::Class<T>::size() > 1u)
			meta::staticFor<1u, meta::Class<T>::size() - 1u, LoadComponent<T>>(component, this, mLayout);

		QPushButton* saveButton{ new QPushButton{ "Save", this } };
		saveButton->setMaximumWidth(120);
		mLayout->addRow(saveButton);

		connect(this, &QObject::destroyed, this, [this, component](QObject *obj) -> void {
			(void) obj;

			if (Service::get<Engine>())
			{
				Asset<T>::erase(component->mGuid);
				Asset<T>::clean();
			}
		} );

		connect(saveButton, &QPushButton::released, this, [this, component]() -> void {
			component->mOnChange();
			component->save(objectName().toStdString().c_str());
		} );
	}

	template<typename T, typename Inspector>
	RegisterAssetInspector<T, Inspector>::RegisterAssetInspector()
	{
		WInspectorAsset::registeredAssetInspector().try_emplace(meta::Class<T>::hash(), [](uint32_t guid, QWidget* parent, QFormLayout* layout) -> WInspectorAsset*
		{
			Inspector* inspector{ new Inspector{ parent, layout } };
			inspector->template load<T>(Asset<T>::add(guid));
			return inspector;
		});
	}
}

#endif