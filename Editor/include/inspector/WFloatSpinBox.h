#ifndef _WFLOAT_SPIN_BOX_H_
#define _WFLOAT_SPIN_BOX_H_

#include <QtWidgets/QDoubleSpinBox> 

#define SIZE_SPIN_BOX 57

#include "IInspectorComponent.h"

namespace maj::editor
{
	class WFloatSpinBox : public QDoubleSpinBox, public IInspectorComponent
	{
	public:
		WFloatSpinBox(float& value, QWidget *parent, std::function<void()> callback = nullptr);
		~WFloatSpinBox();

	private:
		void valueChanged(double value);

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		float& mValue;
	};
}
#endif
