#ifndef _WICCOLLIDER_INSPECTOR_H_
#define _WICCOLLIDER_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <physic/ICCollider.h>

namespace maj::editor
{
	class WICColliderInspector : public WInspectorComponent
	{
	public:
		WICColliderInspector(QWidget* parent, QFormLayout* layout);

		template<typename T>
		void load(T* component);

	private:
		void onTriggerChanged();
		void onDeltaPosChanged();
		void onDeltaRotChanged();
		void onMassChanged();
		void onLinearDampingChanged();
		void onAngularDampingChanged();
		void onGravityChanged();
		void onFreezeChanged();

		physic::ICCollider* mCollider;
	};
}

#include "WICColliderInspector.inl"

#endif