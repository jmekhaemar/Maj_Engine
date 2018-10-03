#ifndef _WINT_SPIN_BOX_H_
#define _WINT_SPIN_BOX_H_

#include <QtWidgets/QSpinBox> 

#define SIZE_SPIN_BOX 57

#include "IInspectorComponent.h"
#include <variant>

#include<QDragEnterEvent>
#include<QDropEvent>

namespace maj::editor
{
	class WIntSpinBox : public QSpinBox, public IInspectorComponent
	{
	public:
		WIntSpinBox(int32_t* value, QWidget *parent, std::function<void()> callback = nullptr);
		WIntSpinBox(uint32_t* value, QWidget *parent, std::function<void()> callback = nullptr);

		~WIntSpinBox();

	private:
		void valueChanged(int value);

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		std::variant<int32_t*, uint32_t*> mValue;
		bool isInteger;

	};
}
#endif
