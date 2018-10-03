#ifndef _WENUM_INSPECTOR_INL_
#define _WENUM_INSPECTOR_INL_

#include "WEnumInspector.h"

namespace maj::editor
{
	template<typename T>
	WEnumInspector<T>::WEnumInspector(T& value, QWidget *parent, std::function<void()> callback)
		: QComboBox{ parent }, IInspectorComponent{ callback }, mValue{ value }
	{
		setAutoFillBackground(false);
		setMaximumWidth(180);

		for (uint32_t i{ 0u }; i < meta::Enum<T>::size(); ++i)
		{
			std::string name{ meta::Enum<T>::getName(i) };
			addItem(name.substr(name.rfind(':') + 1u).c_str());
		}

		setCurrentIndex(static_cast<int>(mValue));

		connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &WEnumInspector<T>::onValueChanged);
		connect(&mTimer, &QTimer::timeout, this, &WEnumInspector::update);
	}

	template<typename T>
	void WEnumInspector<T>::onValueChanged(int value)
	{
		mValue = meta::Enum<T>::getValue(value);
		if (mCallback)
			mCallback();
	}

	template<typename T>
	void WEnumInspector<T>::update()
	{
		if (meta::Enum<T>::getValue(currentIndex()) != mValue && !hasFocus())
			setCurrentIndex(static_cast<int>(mValue));
	}

	template<typename T>
	void WEnumInspector<T>::hideEvent(QHideEvent *event)
	{
		QComboBox::hideEvent(event);
		mTimer.stop();
	}

	template<typename T>
	void WEnumInspector<T>::showEvent(QShowEvent *event)
	{
		QComboBox::showEvent(event);
		mTimer.start(50);
	}
}

#endif