#ifndef _WSTRING_INSPECTOR_H_
#define _WSTRING_INSPECTOR_H_

#include <QLineEdit>

#include "inspector/IInspectorComponent.h"

namespace maj::editor
{
	class WStringInspector : public QLineEdit, public IInspectorComponent
	{
	public:
		WStringInspector(std::string& value, QWidget* parent, std::function<void()> callback = nullptr);
		~WStringInspector();
		
	private:
		void valueChanged();

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		std::string& mValue;
	};
}

#endif