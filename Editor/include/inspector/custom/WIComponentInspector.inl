#ifndef _WICOMPONENT_INSPECTOR_INL_
#define _WICOMPONENT_INSPECTOR_INL_

#include "WIComponentInspector.h"

#include "../WCheckBox.h"

namespace maj::editor
{
	template<typename T>
	void WIComponentInspector::load(T* component)
	{
		mComponent = static_cast<scene::IComponent*>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		WCheckBox* widget{ new WCheckBox(meta::Field<T, 0>::get(mComponent), this, std::bind(&WIComponentInspector::onValueChanged, this)) };

		std::string subStr{ meta::Field<T, 0u>::name() };
		mLayout->addRow(subStr.substr(1).c_str(), widget);
	}
}

#endif