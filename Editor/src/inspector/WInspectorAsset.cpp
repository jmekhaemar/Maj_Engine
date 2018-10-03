#include "inspector/WInspectorAsset.h"

namespace maj::editor
{
	WInspectorAsset::WInspectorAsset(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: WInspectorComponent{ parent, layout, callback }
	{
	}
}