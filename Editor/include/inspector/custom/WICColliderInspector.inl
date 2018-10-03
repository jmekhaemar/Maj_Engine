#ifndef _WICCOLLIDER_INSPECTOR_INL_
#define _WICCOLLIDER_INSPECTOR_INL_

#include "WICColliderInspector.h"

#include "../WCheckBox.h"
#include "../WFloatSpinBox.h"
#include "WAssetInspector.h"

#include <cmath>

namespace maj::editor
{
	template<typename T>
	void WICColliderInspector::load(T* component)
	{
		mCollider = static_cast<physic::ICCollider*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		{
			std::string subStr{ meta::Field<T, 0u>::name() };
			mLayout->setObjectName(subStr.substr(1).c_str());
			WAssetInspector* widget{ new WAssetInspector{ this, mLayout } };
			widget->load<Asset<physic::PhysicMaterial>>(&meta::Field<T, 0u>::get(mCollider));
		}

		{
			std::string subStr{ meta::Field<T, 1u>::name() };
			WFlagInspector<maj::physic::ELayer>* widget{ new WFlagInspector<maj::physic::ELayer>{ meta::Field<T, 1u>::get(mCollider), this } };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WCheckBox* widget{ new WCheckBox(meta::Field<T, 2u>::get(mCollider), this, std::bind(&WICColliderInspector::onTriggerChanged, this)) };
			std::string subStr{ meta::Field<T, 2u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			std::string subStr{ meta::Field<T, 3u>::name() };
			mLayout->setObjectName(subStr.substr(1).c_str());
			WVec3fInspector* widget{ new WVec3fInspector{ this, mLayout, std::bind(&WICColliderInspector::onDeltaPosChanged, this) } };
			widget->load<math::Vec3f>(&meta::Field<T, 3u>::get(mCollider));
		}

		{
			std::string subStr{ meta::Field<T, 4u>::name() };
			mLayout->setObjectName(subStr.substr(1).c_str());
			WQuatfInspector* widget{ new WQuatfInspector{ this, mLayout, std::bind(&WICColliderInspector::onDeltaRotChanged, this) } };
			widget->load<math::Quatf>(&meta::Field<T, 4u>::get(mCollider));
		}

		{
			WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 5u>::get(mCollider), this, std::bind(&WICColliderInspector::onMassChanged, this) } };
			std::string subStr{ meta::Field<T, 5u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 6u>::get(mCollider), this, std::bind(&WICColliderInspector::onLinearDampingChanged, this) } };
			std::string subStr{ meta::Field<T, 6u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 7u>::get(mCollider), this, std::bind(&WICColliderInspector::onAngularDampingChanged, this) } };
			std::string subStr{ meta::Field<T, 7u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WCheckBox* widget{ new WCheckBox(meta::Field<T, 8u>::get(mCollider), this, std::bind(&WICColliderInspector::onGravityChanged, this)) };
			std::string subStr{ meta::Field<T, 8u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WFlagInspector<typename meta::Field<T, 9u>::Type>* widget{ new WFlagInspector<typename meta::Field<T, 9u>::Type>(meta::Field<T, 9u>::get(mCollider), this, std::bind(&WICColliderInspector::onFreezeChanged, this)) };
			std::string subStr{ meta::Field<T, 9u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

	}
}

#endif