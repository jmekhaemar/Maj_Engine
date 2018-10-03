#ifndef _WCWIDGET_INSPECTOR_H_
#define _WCWIDGET_INSPECTOR_H_

#include "inspector/WInspectorComponent.h"

#include <gui/CWidget.h>

namespace maj::editor
{
	class WCWidgetInspector : public WInspectorComponent
	{
	public:
		WCWidgetInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onReferenceChanged();
		void onValueChanged();

		gui::CWidget* mWidget;
	};
}

#include "inspector/custom/WCWidgetInspector.inl"

#endif