#include "WGameContainer.h"

#include <qpainter.h>

#include "Application.h"
#include <Window.h>

namespace maj::editor
{
	WGameContainer::WGameContainer(QWidget *parent, Qt::WindowFlags flags)
		: QDockWidget("Editor", parent, flags), mGame{ QWindow::fromWinId(reinterpret_cast<WId>(Service::get<ww::Window>()->viewport(0u).handle())) }
	{
		mGame->setParent(parent->windowHandle());
		setFocusPolicy(Qt::StrongFocus);
		setFeatures(QDockWidget::NoDockWidgetFeatures);
	}
	
	/*!
	\internal
	*/
	WGameContainer::~WGameContainer()
	{
		// Call destroy() explicitly first. The dtor would do this too, but
		// QEvent::PlatformSurface delivery relies on virtuals. Getting
		// SurfaceAboutToBeDestroyed can be essential for OpenGL, Vulkan, etc.
		// QWindow subclasses in particular. Keep these working.
		//~QWindowContainer();

		delete mGame;
	}

	void WGameContainer::updateGeometry()
	{
		mGame->setGeometry(x(), y() + 20, width(), height() - 20);
	}

	void WGameContainer::focusInEvent(QFocusEvent *event)
	{
		(void) event;
		Service::get<ww::Window>()->focusViewport(0u);
	}

	void WGameContainer::focusOutEvent(QFocusEvent *event)
	{
		(void) event;
	}

	void WGameContainer::keyPressEvent(QKeyEvent* event)
	{
		(void) event;
	}

	bool WGameContainer::event(QEvent* e)
	{
		QEvent::Type type = e->type();
		switch (type) {
		case QEvent::ChildRemoved: {
			QChildEvent *ce = static_cast<QChildEvent *>(e);
			if (ce->child() == mGame)
				mGame = nullptr;
			break;
		}
		case QEvent::Resize:
			updateGeometry();
			break;
		case QEvent::Move:
			updateGeometry();
			break;
		case QEvent::PolishRequest:
			updateGeometry();
			break;
		case QEvent::Show:
			if (mGame->parent())
				mGame->show();
			break;
		case QEvent::Hide:
			if (mGame->parent())
				mGame->hide();
			break;
		case QEvent::Paint:
		{
			QPainter p(this);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.fillRect(x(), y() + 20, width(), height() - 20, Qt::transparent);
			break;
		}
		default:
			break;
		}
		return QDockWidget::event(e);
	}
}