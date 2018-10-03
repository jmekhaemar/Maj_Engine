#ifndef _WCBOX_COLLIDER_INSPECTOR_INL_H
#define _WCBOX_COLLIDER_INSPECTOR_INL_H

#include "WCBoxColliderInspector.h"

namespace maj::editor
{
	template<typename T>
	void WCBoxColliderInspector::load(T* component)
	{
		mCollider = static_cast<physic::CBoxCollider*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		std::string subStr{ meta::Field<T, 0u>::name() };
		mLayout->setObjectName(subStr.substr(1).c_str());

		WVec3fInspector* widget{ new WVec3fInspector{ this, mLayout, std::bind(&WCBoxColliderInspector::onValueChanged, this) } };
		widget->load<math::Vec3f>(&meta::Field<T, 0u>::get(mCollider));
	}
}

#endif