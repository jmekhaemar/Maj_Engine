#include "inspector/WStringInspector.h"

namespace maj::editor
{
	WStringInspector::WStringInspector(std::string& value, QWidget *parent, std::function<void()> callback)
		: QLineEdit{ parent }, IInspectorComponent{ callback }, mValue{ value }
	{
		setMaximumWidth(180);

		setText(value.c_str());

		connect(this, &QLineEdit::editingFinished, this, &WStringInspector::valueChanged);
		connect(&mTimer, &QTimer::timeout, this, &WStringInspector::update);
	}

	WStringInspector::~WStringInspector()
	{}

	void WStringInspector::valueChanged()
	{
		mValue = text().toStdString();
		if (mCallback)
			mCallback();
	}

	void WStringInspector::update()
	{
		if(text().toStdString() != mValue && !hasFocus())
			setText(mValue.c_str());
	}

	void WStringInspector::hideEvent(QHideEvent *event)
	{
		QLineEdit::hideEvent(event);
		mTimer.stop();
	}

	void WStringInspector::showEvent(QShowEvent *event)
	{
		QLineEdit::showEvent(event);
		mTimer.start(50);
	}
}