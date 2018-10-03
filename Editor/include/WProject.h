#ifndef _WPROJECT_H_
#define _WPROJECT_H_

#include <QtWidgets/QDockWidget>

#include <QtWidgets/QFileSystemModel>
#include <QtWidgets/QTreeView>
#include <QString>
#include <QTreeView>
#include <QEvent>

namespace maj::editor
{ 
	class WMainWindow;
	class WProject : public QDockWidget
	{
		class WTreeView : public QTreeView
		{
		public:
			WTreeView(WProject* parent);
			~WTreeView();
			void removeScene();
		protected:
			bool eventFilter(QObject* obj, QEvent* event);
			WProject* mParent;
		};

	public:
		WProject(const QString& kTitle, WMainWindow* parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
		~WProject();

		const QFileSystemModel* fileSystem() const;
		QTreeView* treeView();

		void showContextMenu(const QPoint& kPos);

		void createScene();
		void renameScene(const QString& kPath, const QString& kOldName, const QString& kNewName);

		void createModel();
		void createSkelModel();
		void createTexture();
		void createImage();
		void createFont();
		void createMaterial();
		void createMatLayout();
		void createPhysicMat();
		void canRighClick();

	protected:
		//void mousePressEvent(QMouseEvent *event);
	
	private:
		WMainWindow* mParent;
		QFileSystemModel* mDirModel;
		WTreeView* mTreeView;
		QPoint mCurrentModelIndex;
		std::string mFuturName;
	};
}

#endif