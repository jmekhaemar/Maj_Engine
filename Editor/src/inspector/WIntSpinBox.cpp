#include "inspector/WIntSpinBox.h"

#include <limits>
#include "Logging.h"

namespace maj::editor
{
	WIntSpinBox::WIntSpinBox(int32_t* value, QWidget *parent, std::function<void()> callback)
		:QSpinBox{ parent }, IInspectorComponent{ callback }, mValue{ value }, isInteger{ true }
	{
		setButtonSymbols(QAbstractSpinBox::NoButtons);
		setMaximum(std::numeric_limits<int32_t>::max());
		setMinimum(std::numeric_limits<int32_t>::lowest());
		setMaximumWidth(SIZE_SPIN_BOX);
		setAutoFillBackground(false);
		setValue(*value);

		connect(this, QOverload<int>::of(&QSpinBox::valueChanged), this, &WIntSpinBox::valueChanged);
		connect(&mTimer, &QTimer::timeout, this, &WIntSpinBox::update);
	}

	WIntSpinBox::WIntSpinBox(uint32_t* value, QWidget *parent, std::function<void()> callback)
		:QSpinBox{ parent }, IInspectorComponent{ callback }, mValue{ value }, isInteger{ false }
	{
		setButtonSymbols(QAbstractSpinBox::NoButtons);
		setMaximum(std::numeric_limits<int32_t>::max());
		setMinimum(0);
		setMaximumWidth(SIZE_SPIN_BOX);
		setAutoFillBackground(false);
		setValue(static_cast<int32_t>(*value));

		connect(this, QOverload<int>::of(&QSpinBox::valueChanged), this, &WIntSpinBox::valueChanged);
		connect(&mTimer, &QTimer::timeout, this, &WIntSpinBox::update);
	}

	WIntSpinBox::~WIntSpinBox()
	{
	}

	void WIntSpinBox::valueChanged(int value)
	{
		if (isInteger)
			*(std::get<int32_t*>(mValue)) =  value;
		else
			*(std::get<uint32_t*>(mValue)) =  static_cast<uint32_t>(value);
		if (mCallback)
			mCallback();
	}

	void WIntSpinBox::update()
	{
		if (!hasFocus())
		{
			if (isInteger && value() != *(std::get<int32_t*>(mValue)))
				setValue(*(std::get<int32_t*>(mValue)));
			else if(value() != *(std::get<uint32_t*>(mValue)))
				setValue(*(std::get<uint32_t*>(mValue)));
		}
	}

	void WIntSpinBox::hideEvent(QHideEvent *event)
	{
		QSpinBox::hideEvent(event);
		mTimer.stop();
	}

	void WIntSpinBox::showEvent(QShowEvent *event)
	{
		QSpinBox::showEvent(event);
		mTimer.start(50);
	}
}