#ifndef _WSHADER_INSPECTOR_INL_
#define _WSHADER_INSPECTOR_INL_

#include "WShaderInspector.h"

#include "../WStringInspector.h"
#include "../WFlagInspector.h"

namespace maj::editor
{
	template<typename T>
	void WShaderInspector::load(T* component)
	{
		mShader = static_cast<rhi::Shader*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		{
			WStringInspector* widget{ new WStringInspector{ meta::Field<T, 0u>::get(mShader), this, std::bind(&WShaderInspector::onValueChanged, this) } };
			std::string subStr{ meta::Field<T, 0u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WFlagInspector<typename meta::Field<T, 1u>::Type>* widget{ new WFlagInspector<typename meta::Field<T, 1u>::Type>{ meta::Field<T, 1u>::get(mShader), this } };
			std::string subStr{ meta::Field<T, 1u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}
	}
}

#endif