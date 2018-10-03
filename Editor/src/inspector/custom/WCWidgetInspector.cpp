#include "inspector/custom/WCWidgetInspector.h"

namespace maj::editor
{
	WCWidgetInspector::WCWidgetInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent { parent, layout }
	{}

	void WCWidgetInspector::onReferenceChanged()
	{
		mWidget->reference(mWidget->reference());
	}

	void WCWidgetInspector::onValueChanged()
	{
		mWidget->space(mWidget->space());
	}
}