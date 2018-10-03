#ifndef _WVEC4F_INSPECTOR_INL_
#define _WVEC4F_INSPECTOR_INL_

#include <QBoxLayout>

#include "WVec4fInspector.h"

namespace maj::editor
{
	template<typename T>
	void WVec4fInspector::load(T* component)
	{
		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		math::Vec4f* vec{ static_cast<math::Vec4f*>(component) };
		QHBoxLayout* layout{ new QHBoxLayout{} };

		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{ (*vec)[0], this, mCallback } };
			formLayout->addRow("X", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{ (*vec)[1], this, mCallback } };
			formLayout->addRow("Y", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{ (*vec)[2], this, mCallback } };
			formLayout->addRow("Z", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{ (*vec)[3], this, mCallback } };
			formLayout->addRow("W", widget);
			layout->addLayout(formLayout);
		}

		mLayout->addRow(mLayout->objectName(), layout);
	}
}

#endif