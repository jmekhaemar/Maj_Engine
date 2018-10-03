#ifndef _WHIERARCHY_H_
#define _WHIERARCHY_H_

#include <QtWidgets/QDockWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QString>

#include "scene/Scene.h"
#include "scene/Actor.h"

namespace maj::editor
{
	class WMainWindow;
	class WHierarchy : public QDockWidget
	{
	private:
		class WTreeWidget : public QTreeWidget
		{
		public:
			WTreeWidget(WHierarchy* parent = 0);
			virtual ~WTreeWidget();

			void updateSelection();
			void removeActor();
		protected:
			virtual void mousePressEvent(QMouseEvent* event) override;
			bool eventFilter(QObject* obj, QEvent* event)override;
			WHierarchy* mParent;
		};

	public:
		WHierarchy(const QString& kTitle, WMainWindow* parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
		~WHierarchy();

		void update(const QModelIndex& kModel);
		void load(scene::Scene* activeScene);

		void showContextMenu(const QPoint& kPos);

		void addActor();
		void addACamera();
		void addASkybox();
		void removeActor();
		void rename(const QString &kPath, const QString &kOldName, const QString &kNewName);

		QTreeWidget* treeWidget();
		void canRighClick();

	private:
		void browse(const scene::Actor* kActor, QTreeWidgetItem* parentWidget = Q_NULLPTR);

		WTreeWidget* mTree;
		WMainWindow* mParent;
		std::string mFuturName;
	};
}

#endif