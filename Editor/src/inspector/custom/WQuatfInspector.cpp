#include "inspector/custom/WQuatfInspector.h"

namespace maj::editor
{
	WQuatfInspector::WQuatfInspector(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: WInspectorComponent{ parent, layout, callback }
	{
		connect(&mTimer, &QTimer::timeout, this, &WQuatfInspector::update);
	}

	void WQuatfInspector::onValueChanged()
	{
		(*mQuat) = math::Quatf::fromEuler(mEuler);
		if (mCallback)
			mCallback();
	}

	void WQuatfInspector::update()
	{
		mEuler = mQuat->toEuler();
	}

	void WQuatfInspector::hideEvent(QHideEvent *event)
	{
		WInspectorComponent::hideEvent(event);
		mTimer.stop();
	}

	void WQuatfInspector::showEvent(QShowEvent *event)
	{
		WInspectorComponent::showEvent(event);
		mTimer.start(100);
	}
}