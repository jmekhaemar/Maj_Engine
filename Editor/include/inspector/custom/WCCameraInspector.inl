#ifndef _WCCAMERA_INSPECTOR_INL_
#define _WCCAMERA_INSPECTOR_INL_

#include "WCCameraInspector.h"

namespace maj::editor
{
	template<typename T>
	void WCCameraInspector::load(T* component)
	{
		mCamera = static_cast<scene::CCamera*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		{
			QHBoxLayout* layout{ new QHBoxLayout{} };
			{
				QFormLayout* formLayout{ new QFormLayout{} };
				WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 1u>::get(mCamera), this, std::bind(&WCCameraInspector::onValueChanged, this) } };
				std::string subStr{ meta::Field<T, 1u>::name() };
				formLayout->addRow(subStr.substr(1).c_str(), widget);
				layout->addLayout(formLayout);
			}
			{
				QFormLayout* formLayout{ new QFormLayout{} };
				WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 2u>::get(mCamera), this, std::bind(&WCCameraInspector::onValueChanged, this) } };
				std::string subStr{ meta::Field<T, 2u>::name() };
				formLayout->addRow(subStr.substr(1).c_str(), widget);
				layout->addLayout(formLayout);
			}
			mLayout->addRow(layout);
		}

		{
			QHBoxLayout* layout{ new QHBoxLayout{} };
			{
				QFormLayout* formLayout{ new QFormLayout{} };
				WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 0u>::get(mCamera), this, std::bind(&WCCameraInspector::onValueChanged, this) } };
				std::string subStr{ meta::Field<T, 0u>::name() };
				formLayout->addRow(subStr.substr(1).c_str(), widget);
				layout->addLayout(formLayout);
			}
			{
				QFormLayout* formLayout{ new QFormLayout{} };
				WFloatSpinBox* widget{ new WFloatSpinBox{ meta::Field<T, 3u>::get(mCamera), this, std::bind(&WCCameraInspector::onValueChanged, this) } };
				std::string subStr{ meta::Field<T, 3u>::name() };
				formLayout->addRow(subStr.substr(1).c_str(), widget);
				layout->addLayout(formLayout);
			}
			mLayout->addRow(layout);
		}
	}
}

#endif