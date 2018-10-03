#ifndef _WVEC3F_INSPECTOR_INL_
#define _WVEC3F_INSPECTOR_INL_

#include <QBoxLayout>

#include "inspector/custom/WVec3fInspector.h"

namespace maj::editor
{
	template<typename T>
	void WVec3fInspector::load(T* component)
	{
		std::string name { meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		math::Vec3f* vec{ static_cast<math::Vec3f*>(component) };
		QHBoxLayout* layout{ new QHBoxLayout{} };
		
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{(*vec)[0], this, mCallback} };
			formLayout->addRow("X", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{(*vec)[1], this, mCallback } };
			formLayout->addRow("Y", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WFloatSpinBox* widget{ new WFloatSpinBox{(*vec)[2], this, mCallback } };
			formLayout->addRow("Z", widget);
			layout->addLayout(formLayout);
		}

		mLayout->addRow(mLayout->objectName(), layout);
	}
}

#endif