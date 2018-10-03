#ifndef _WQUATF_INSPECTOR_INL_
#define _WQUATF_INSPECTOR_INL_

#include "WQuatfInspector.h"

#include <functional>

namespace maj::editor
{
	template<typename T>
	void WQuatfInspector::load(T* component)
	{
		mQuat = static_cast<math::Quatf*>(component);
		mEuler = mQuat->toEuler();

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		QHBoxLayout* layout{ new QHBoxLayout{} };

		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{ mEuler.mX, this, std::bind(&WQuatfInspector::onValueChanged, this) } };
			formLayout->addRow("X", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{ mEuler.mY, this, std::bind(&WQuatfInspector::onValueChanged, this) } };
			formLayout->addRow("Y", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{ mEuler.mZ, this, std::bind(&WQuatfInspector::onValueChanged, this) } };
			formLayout->addRow("Z", widget);
			layout->addLayout(formLayout);
		}

		mLayout->addRow(mLayout->objectName(), layout);
	}
}

#endif