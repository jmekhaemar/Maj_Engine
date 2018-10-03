#include "inspector/WDropInspector.h"
#include "Logging.h"


namespace maj::editor
{
	WDropInspector::WDropInspector(std::string& value, QWidget *parent, std::function<void(QDragEnterEvent* event)> validateDrop, std::function<void(QDropEvent *event)> executeDrop)
		: WStringInspector{ value, parent }, mValidateDrop{ validateDrop }, mExecuteDrop{ executeDrop }
	{
		setAcceptDrops(true);
		setReadOnly(true);
	}

	WDropInspector::~WDropInspector()
	{
	}

	void WDropInspector::dragEnterEvent(QDragEnterEvent *event)
	{
		if (mValidateDrop)
			mValidateDrop(event);
	}

	void WDropInspector::dropEvent(QDropEvent *event)
	{
		if (mExecuteDrop)
			mExecuteDrop(event);
	}
}