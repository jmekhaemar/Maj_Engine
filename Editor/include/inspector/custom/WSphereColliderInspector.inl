#ifndef _WSPHERE_COLLIDER_INSPECTOR_INL_
#define _WSPHERE_COLLIDER_INSPECTOR_INL_

#include "WSphereColliderInspector.h"

namespace maj::editor
{
	template<typename T>
	void WSphereColliderInspector::load(T* component)
	{
		mCollider = static_cast<physic::CSphereCollider*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 0u>::get(mCollider), this, std::bind(&WSphereColliderInspector::onValueChanged, this) } };

		std::string subStr{ meta::Field<T, 0u>::name() };
		mLayout->addRow(subStr.substr(1).c_str(), widget);
	}
}

#endif