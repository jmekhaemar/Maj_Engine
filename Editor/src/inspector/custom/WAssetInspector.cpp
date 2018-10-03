#include "inspector/custom/WAssetInspector.h"


namespace maj::editor
{
	WAssetInspector::WAssetInspector(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: WInspectorComponent{ parent, layout, callback }
	{
		setAcceptDrops(true);
	}
}