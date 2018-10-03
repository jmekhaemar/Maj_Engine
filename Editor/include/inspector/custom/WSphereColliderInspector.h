#ifndef _WSPHERE_COLLIDER_INSPECTOR_H_
#define _WSPHERE_COLLIDER_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <physic/CSphereCollider.h>

namespace maj::editor
{
	class WSphereColliderInspector : public WInspectorComponent
	{
	public:
		WSphereColliderInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);
	
	private:
		void onValueChanged();

		physic::CSphereCollider* mCollider;
	};
}

#include "WSphereColliderInspector.inl"

#endif