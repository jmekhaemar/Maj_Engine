#ifndef _WINSPECTOR_H_
#define _WINSPECTOR_H_

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBox>
#include <QString>
#include <QTreeWidget>

#include <scene/Actor.h>

namespace maj::editor
{
	class WMainWindow;
	class WInspector : public QDockWidget
	{
	public:
		WInspector(const QString& kTitle, WMainWindow* parent = nullptr, Qt::WindowFlags flags = 0);
		WInspector(QWidget* parent = 0, Qt::WindowFlags flags = 0);

		void updateAsset(const QModelIndex& kModel);
		void updateScene(QTreeWidgetItem* current, QTreeWidgetItem* previous);
		void selectionChanged();

		void showContextMenu(const QPoint& kPos);
		void addTransform();
		void removeComponent();
		void clearToolBox();

		bool inspectActor() const;
		void canRighClick();
	private:
		void loadActor();
		void loadAsset(const std::string& kName);

		WMainWindow* mParent;
		QToolBox* mTool;
		scene::Actor* mCurrentActor;
		int16_t mTransfIndex;
	};

}

#endif