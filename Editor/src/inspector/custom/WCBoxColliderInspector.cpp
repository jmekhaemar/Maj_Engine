#include "inspector/custom/WCBoxColliderInspector.h"

namespace maj::editor
{
	WCBoxColliderInspector::WCBoxColliderInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{
	}

	void WCBoxColliderInspector::onValueChanged()
	{
		mCollider->halfExtent(meta::Field<physic::CBoxCollider, 0u>::get(mCollider));
	}
}