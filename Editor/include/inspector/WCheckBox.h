#ifndef _WCHECK_BOX_H_
#define _WCHECK_BOX_H_

#include <QCheckBox>

#include "IInspectorComponent.h"

namespace maj::editor
{
	class WCheckBox : public QCheckBox, public IInspectorComponent
	{
	public:
		WCheckBox(bool& value, QWidget *parent, std::function<void()> callback = nullptr);
		~WCheckBox();

	private:
		void valueChanged(int value);

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		bool& mValue;
	};
}
#endif



