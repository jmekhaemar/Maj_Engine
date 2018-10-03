#ifndef _WCCAPSULE_COLLIDER_INSPECTOR_H_
#define _WCCAPSULE_COLLIDER_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <physic/CCapsuleCollider.h>

namespace maj::editor
{
	class WCCapsuleColliderInspector : public WInspectorComponent
	{
	public:
		WCCapsuleColliderInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onRadiusChanged();
		void onHalfHeightChanged();

		physic::CCapsuleCollider* mCollider;
	};
}

#include "WCCapsuleColliderInspector.inl"

#endif