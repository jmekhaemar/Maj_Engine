#ifndef _WVEC2UI_INSPECTOR_INL_
#define _WVEC2UI_INSPECTOR_INL_

#include "inspector/custom/WVec2uiInspector.h"

#include <QBoxLayout>

#include "inspector/WIntSpinBox.h"

namespace maj::editor
{
	template<typename T>
	void WVec2uiInspector::load(T* component)
	{
		std::string name { meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		math::Vec2ui* vec { static_cast<math::Vec2ui*>(component) };
		QHBoxLayout* layout { new QHBoxLayout {} };

		{
			QFormLayout* formLayout { new QFormLayout {} };
			WIntSpinBox* widget { new WIntSpinBox { &(*vec)[0u], this, mCallback } };
			formLayout->addRow("X", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout { new QFormLayout {} };
			WIntSpinBox* widget { new WIntSpinBox { &(*vec)[1u], this, mCallback } };
			formLayout->addRow("Y", widget);
			layout->addLayout(formLayout);
		}

		mLayout->addRow(mLayout->objectName(), layout);
	}
}
#endif
