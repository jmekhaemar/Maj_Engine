#ifndef _WENUM_INSPECTOR_H_
#define _WENUM_INSPECTOR_H_

#include <QComboBox>

#include "IInspectorComponent.h"

namespace maj::editor
{
	template<typename T>
	class WEnumInspector : public QComboBox, public IInspectorComponent
	{
	public:
		WEnumInspector(T& value, QWidget *parent, std::function<void()> callback = nullptr);

	private:
		void onValueChanged(int value);

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		T& mValue;
	};
}

#include "WEnumInspector.inl"

#endif