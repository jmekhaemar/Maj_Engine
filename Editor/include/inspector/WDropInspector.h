#ifndef _WDROP_INSPECTOR_H_
#define _WDROP_INSPECTOR_H_

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "WStringInspector.h"

namespace maj::editor
{
	class WDropInspector : public WStringInspector
	{
	public:
		WDropInspector(std::string& value, QWidget *parent, std::function<void(QDragEnterEvent* event)> validateDrop = nullptr, std::function<void(QDropEvent *event)> executeDrop = nullptr);
		~WDropInspector();

	protected:
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
		std::function<void(QDragEnterEvent* event)> mValidateDrop;
		std::function<void(QDropEvent *event)> mExecuteDrop;
	};
}

#endif