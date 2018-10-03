#ifndef _WCCAPSULE_COLLIDER_INSPECTOR_INL_
#define _WCCAPSULE_COLLIDER_INSPECTOR_INL_

#include "WCCapsuleColliderInspector.h"

namespace maj::editor
{
	template<typename T>
	void WCCapsuleColliderInspector::load(T* component)
	{
		mCollider = static_cast<physic::CCapsuleCollider*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		{
			WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 0u>::get(mCollider), this, std::bind(&WCCapsuleColliderInspector::onRadiusChanged, this) } };
			std::string subStr{ meta::Field<T, 0u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}
		{
			WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 1u>::get(mCollider), this, std::bind(&WCCapsuleColliderInspector::onHalfHeightChanged, this) } };
			std::string subStr{ meta::Field<T, 1u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}
	}
}

#endif