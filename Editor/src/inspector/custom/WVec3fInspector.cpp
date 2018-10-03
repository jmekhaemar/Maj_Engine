#include "inspector/custom/WVec3fInspector.h"

namespace maj::editor
{
	WVec3fInspector::WVec3fInspector(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: WInspectorComponent{ parent, layout, callback }
	{}
}