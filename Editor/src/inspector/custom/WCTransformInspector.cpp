#include "inspector/custom/WCTransformInspector.h"

namespace maj::editor
{
	WCTransformInspector::WCTransformInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{
	}

	void WCTransformInspector::onValueChanged()
	{
		mTransform->mOnChanged();
	}

	void WCTransformInspector::onStaticChanged()
	{
		mTransform->mOnStaticChanged(mTransform->isStatic());
	}
}