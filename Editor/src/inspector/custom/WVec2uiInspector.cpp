#include "inspector/custom/WVec2uiInspector.h"

namespace maj::editor
{
	WVec2uiInspector::WVec2uiInspector(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: WInspectorComponent { parent, layout, callback }
	{}
}