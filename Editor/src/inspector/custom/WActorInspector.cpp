#include "inspector/custom/WActorInspector.h"

#include "Application.h"
#include "WMainWindow.h"

namespace maj::editor
{
	WActorInspector::WActorInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{
	}

	void WActorInspector::onNameChanged()
	{
		if (mOldName == mActor->name())
			return;

		Application* tempAppli{ static_cast<Application*>(QApplication::instance()) };
		WMainWindow* main{ tempAppli->mainWindow() };
		main->hierarchy()->treeWidget()->findItems(mOldName.c_str(), Qt::MatchRecursive)[0]->setData(0, 0, mActor->name().c_str());
		mOldName = mActor->name();

	}
}