#include "inspector/custom/WSphereColliderInspector.h"

namespace maj::editor
{
	WSphereColliderInspector::WSphereColliderInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{
	}

	void WSphereColliderInspector::onValueChanged()
	{
		mCollider->radius(meta::Field<physic::CSphereCollider, 0u>::get(mCollider));
	}
}