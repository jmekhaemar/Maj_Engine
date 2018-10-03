#ifndef _WACTOR_INSPECTOR_INL_
#define _WACTOR_INSPECTOR_INL_

#include "WActorInspector.h"

#include "../WStringInspector.h"

namespace maj::editor
{
	template<typename T>
	void WActorInspector::load(T* component)
	{
		mActor = static_cast<scene::Actor*>(component);

		std::string name{ meta::Class<T>::name() };
		mOldName = meta::Field<T, 3u>::get(component);

		setObjectName(name.substr(name.rfind(':') + 1u).c_str());
		{
			std::string subStr{ meta::Field<T, 3u>::name() };

			WStringInspector* widget{ new WStringInspector{ meta::Field<T, 3u>::get(component), this, std::bind(&WActorInspector::onNameChanged, this) } };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}
	}
}

#endif