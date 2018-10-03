#include "inspector/custom/WIComponentInspector.h"

namespace maj::editor
{
	WIComponentInspector::WIComponentInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{}

	void WIComponentInspector::onValueChanged()
	{
		bool isEnable{ meta::Field<scene::IComponent, 0u>::get(mComponent) };
		if (isEnable)
			mComponent->mOnEnabled();
		else
			mComponent->mOnDisabled(false);
	}
}