
#include "inspector/WFloatSpinBox.h"

#include <limits>

namespace maj::editor
{
	WFloatSpinBox::WFloatSpinBox(float& value, QWidget *parent, std::function<void()> callback)
		: QDoubleSpinBox{ parent }, IInspectorComponent{ callback }, mValue{ value }
	{
		setButtonSymbols(QAbstractSpinBox::NoButtons);
		setDecimals(7);
		setMaximum(std::numeric_limits<double>::max());
		setMinimum(std::numeric_limits<double>::lowest());
		setMaximumWidth(SIZE_SPIN_BOX);

		setValue(value);

		connect(this, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &WFloatSpinBox::valueChanged);
		connect(&mTimer, &QTimer::timeout, this, &WFloatSpinBox::update);
	}

	WFloatSpinBox::~WFloatSpinBox()
	{
	}

	void WFloatSpinBox::valueChanged(double value)
	{
		mValue = static_cast<float>(value);
		if (mCallback)
			mCallback();
	}

	void WFloatSpinBox::update()
	{
		if (value() != mValue && !hasFocus())
			setValue(mValue);
	}

	void WFloatSpinBox::hideEvent(QHideEvent *event)
	{
		QDoubleSpinBox::hideEvent(event);
		mTimer.stop();
	}

	void WFloatSpinBox::showEvent(QShowEvent *event)
	{
		QDoubleSpinBox::showEvent(event);
		mTimer.start(50);
	}
}