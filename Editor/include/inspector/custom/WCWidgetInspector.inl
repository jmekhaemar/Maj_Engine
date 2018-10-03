#ifndef _WCWIDGET_INSPECTOR_INL_
#define _WCWIDGET_INSPECTOR_INL_

#include "inspector/custom/WCWidgetInspector.h"

namespace maj::editor
{
	template <typename T>
	void WCWidgetInspector::load(T* component)
	{
		mWidget = static_cast<gui::CWidget*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name { meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		{
			WEnumInspector<typename meta::Field<T, 0u>::Type>* widget { new WEnumInspector { meta::Field<T, 0u>::get(mWidget), this, std::bind(&WCWidgetInspector::onValueChanged, this) } };
			std::string subStr { meta::Field<T, 0u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WVec2uiInspector* widget { new WVec2uiInspector { this, nullptr, std::bind(&WCWidgetInspector::onReferenceChanged, this) } };
			widget->load<math::Vec2ui>(&meta::Field<T, 1u>::get(mWidget));
			std::string subStr { meta::Field<T, 1u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WVectorInspector<std::vector<gui::IElement*>>* widget { new WVectorInspector{ meta::Field<T, 2u>::get(mWidget), this } };
			std::string subStr { meta::Field<T, 2u>::name() };
			widget->setTitle(subStr.substr(1).c_str());
			mLayout->addRow(widget);
		}
	}
}

#endif