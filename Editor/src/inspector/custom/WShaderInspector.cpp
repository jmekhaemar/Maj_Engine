#include "inspector/custom/WShaderInspector.h"

namespace maj::editor
{
	WShaderInspector::WShaderInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{
	}

	void WShaderInspector::onValueChanged()
	{
		mShader->initialize();
	}
}