#include "inspector/custom/WVec4fInspector.h"

namespace maj::editor
{
	WVec4fInspector::WVec4fInspector(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: WInspectorComponent{ parent, layout, callback }
	{}
}