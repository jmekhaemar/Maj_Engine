#ifndef _WCTRANSFORM_INSPECTOR_INL_
#define _WCTRANSFORM_INSPECTOR_INL_

#include "WCTransformInspector.h"

#include "WVec3fInspector.h"
#include "WQuatfInspector.h"

namespace maj::editor
{
	template<typename T>
	void WCTransformInspector::load(T* component)
	{
		mTransform = static_cast<scene::CTransform*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		{
			std::string subStr{ meta::Field<typename meta::Field<T, 0u>::Type, 0u>::name() };
			mLayout->setObjectName(subStr.substr(1).c_str());
			WVec3fInspector* pos{ new WVec3fInspector{ this, mLayout, std::bind(&WCTransformInspector::onValueChanged, this) } };
			pos->load<math::Vec3f>(const_cast<math::Vec3f*>(&(meta::Field<T, 0u>::get(mTransform).position())));
		}

		{
			std::string subStr{ meta::Field<typename meta::Field<T, 0u>::Type, 1u>::name() };
			mLayout->setObjectName(subStr.substr(1).c_str());
			WQuatfInspector* quat{ new WQuatfInspector{ this, mLayout, std::bind(&WCTransformInspector::onValueChanged, this) } };
			quat->load<math::Quatf>(const_cast<math::Quatf*>(&(meta::Field<T, 0u>::get(mTransform).rotation())));
		}

		{
			std::string subStr{ meta::Field<typename meta::Field<T, 0u>::Type, 2u>::name() };
			mLayout->setObjectName(subStr.substr(1).c_str());
			WVec3fInspector* scale{ new WVec3fInspector{ this, mLayout, std::bind(&WCTransformInspector::onValueChanged, this) } };
			scale->load<math::Vec3f>(const_cast<math::Vec3f*>(&(meta::Field<T, 0u>::get(mTransform).scale())));
		}

		WCheckBox* widget{ new WCheckBox{ meta::Field<T, 1u>::get(mTransform), this, std::bind(&WCTransformInspector::onStaticChanged, this) } };
		std::string subStr{ meta::Field<T, 1u>::name() };
		mLayout->addRow(subStr.substr(1).c_str(), widget);
	}
}

#endif