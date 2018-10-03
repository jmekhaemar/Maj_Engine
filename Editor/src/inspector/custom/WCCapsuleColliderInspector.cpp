#include "inspector/custom/WCCapsuleColliderInspector.h"

namespace maj::editor
{
	WCCapsuleColliderInspector::WCCapsuleColliderInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{
	}

	void WCCapsuleColliderInspector::onRadiusChanged()
	{
		mCollider->radius(meta::Field<physic::CCapsuleCollider, 0u>::get(mCollider));
	}

	void WCCapsuleColliderInspector::onHalfHeightChanged()
	{
		mCollider->halfHeight(meta::Field<physic::CCapsuleCollider, 1u>::get(mCollider));
	}
}