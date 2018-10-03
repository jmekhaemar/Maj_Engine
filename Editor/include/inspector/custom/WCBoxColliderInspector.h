#ifndef _WCBOX_COLLIDER_INSPECTOR_H_
#define _WCBOX_COLLIDER_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <physic/CBoxCollider.h>

namespace maj::editor
{
	class WCBoxColliderInspector : public WInspectorComponent
	{
	public:
		WCBoxColliderInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onValueChanged();

		physic::CBoxCollider* mCollider;
	};
}

#include "WCBoxColliderInspector.inl"

#endif