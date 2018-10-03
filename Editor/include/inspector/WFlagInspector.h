#ifndef _WFLAG_INSPECTOR_H_
#define _WFLAG_INSPECTOR_H_

#include <QComboBox>

#include "IInspectorComponent.h"

namespace maj::editor
{
	template<typename T>
	class WFlagInspector : public QComboBox, public IInspectorComponent
	{
	public:
		WFlagInspector(T& value, QWidget* parent, std::function<void()> callback = nullptr);

	private:
		void onValueChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		T& mValue;
	};
}

#include "WFlagInspector.inl"

#endif