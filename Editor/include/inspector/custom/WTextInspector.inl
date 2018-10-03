#ifndef _WTEXT_INSPECTOR_INL_
#define _WTEXT_INSPECTOR_INL_

#include "WTextInspector.h"

namespace maj::editor
{
	template <typename T>
	void WTextInspector::load(T* component)
	{
		mText = static_cast<gui::Text*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name { meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());
	}
}

#endif