#ifndef _IINSPECTOR_COMPONENT_H_
#define _IINSPECTOR_COMPONENT_H_

#include <QtWidgets/qwidget.h>
#include <qtimer.h>

#include <functional>

namespace maj::editor
{
	class IInspectorComponent
	{
	public:
		IInspectorComponent(std::function<void()> callback = nullptr);

	protected:
		virtual void update();
		virtual void hideEvent(QHideEvent *event) = 0;
		virtual void showEvent(QShowEvent *event) = 0;

		QTimer mTimer;
		std::function<void()> mCallback;
	};
}

#endif