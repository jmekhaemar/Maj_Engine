#ifndef _WASSET_INSPECTOR_INL_
#define _WASSET_INSPECTOR_INL_

#include "WAssetInspector.h"
#include "../WDropInspector.h"
#include <Asset.h>

#include <QMimeData>

#include "Application.h"

namespace maj::editor
{ 
	template<typename T>
	void WAssetInspector::load(T* component)
	{
		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		mName = T::name(component->mGuid);
		
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		WDropInspector* widget{ new WDropInspector{ mName, this, [this](QDragEnterEvent* event) -> void
		{
			if (event->mimeData()->hasText())
			{
				std::string strTest{ event->mimeData()->text().toStdString() };
				strTest = strTest.substr(strTest.find('C'));

				std::unique_ptr<JsonNode> root{ JsonNode::parse(strTest.c_str()) };
				uint32_t type{ (*root)["type"]->getUint32() };

				if(meta::Class<typename T::AssetType>::hash() == type)
					event->acceptProposedAction();
				else
					event->ignore();
			}
			else
				event->ignore();
		} ,
		[this, component](QDropEvent *event) -> void
		{
			std::string strTest{ event->mimeData()->text().toStdString() };
			strTest = strTest.substr(strTest.find('C'));

			std::unique_ptr<JsonNode> root{ JsonNode::parse(strTest.c_str()) };
			uint32_t guid{ (*root)["mGuid"]->getUint32() };

			(*component) = guid;
			mName = T::name(guid);
			Application* app{ static_cast<Application*>(QApplication::instance()) };
			if (app->mainWindow()->inspector()->inspectActor())
				component->change();
		}
		} };

		mLayout->addRow(mLayout->objectName(), widget);
	}
}

#endif