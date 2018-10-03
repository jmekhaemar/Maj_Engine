#include "inspector/WCheckBox.h"

namespace maj::editor
{
	WCheckBox::WCheckBox(bool& value, QWidget *parent, std::function<void()> callback)
		:QCheckBox{ parent }, IInspectorComponent{ callback }, mValue { value }
	{
		setAutoFillBackground(false);
		setChecked(value);
		connect(this, &WCheckBox::toggled, this, &WCheckBox::valueChanged);
		connect(&mTimer, &QTimer::timeout, this, &WCheckBox::update);
	}

	WCheckBox::~WCheckBox()
	{
	}

	void WCheckBox::valueChanged(int value)
	{
		mValue = static_cast<bool>(value);
		if (mCallback)
			mCallback();
	}

	void WCheckBox::update()
	{
		if (isChecked() != mValue && !hasFocus())
			setChecked(mValue);
	}

	void WCheckBox::hideEvent(QHideEvent *event)
	{
		QWidget::hideEvent(event);
		mTimer.stop();
	}

	void WCheckBox::showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);
		mTimer.start(50);
	}
}