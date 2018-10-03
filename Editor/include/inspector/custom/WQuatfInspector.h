#ifndef _WQUATF_INSPECTOR_H_
#define _WQUATF_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <QTimer>
#include <Quaternion.h>

namespace maj::editor
{
	class WQuatfInspector : public WInspectorComponent
	{
	public:
		WQuatfInspector(QWidget* parent, QFormLayout* layout = nullptr, std::function<void()> callback = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onValueChanged();

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		math::Quatf* mQuat;
		math::Vec3f mEuler;
	};
}

#include "WQuatfInspector.inl"

#endif