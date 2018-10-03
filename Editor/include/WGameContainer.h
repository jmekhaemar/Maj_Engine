#ifndef _WGAME_CONTAINER_H_
#define _WGAME_CONTAINER_H_

#include <QtWidgets/QDockWidget>
#include <QtGui/qevent.h>
#include <qwindow.h>

namespace maj::editor
{
	class WGameContainer : public QDockWidget
	{
	public:
		explicit WGameContainer(QWidget *parent = 0, Qt::WindowFlags f = 0);
		~WGameContainer();

	private:
		void updateGeometry();

		void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
		void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

		void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

		bool event(QEvent* e) Q_DECL_OVERRIDE;

		QWindow* mGame;
	};
}

#endif 