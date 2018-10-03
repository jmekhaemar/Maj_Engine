#include "inspector/custom/WICColliderInspector.h"

namespace maj::editor
{
	WICColliderInspector::WICColliderInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{
	}

	void WICColliderInspector::onTriggerChanged()
	{
		mCollider->isTrigger(meta::Field<physic::ICCollider, 2u>::get(mCollider));
	}

	void WICColliderInspector::onDeltaPosChanged()
	{
		mCollider->deltaPosition(mCollider->deltaPosition());
	}

	void WICColliderInspector::onDeltaRotChanged()
	{
		mCollider->deltaRotation(mCollider->deltaRotation());
	}

	void WICColliderInspector::onMassChanged()
	{
		mCollider->mass(meta::Field<physic::ICCollider, 5u>::get(mCollider));
	}

	void WICColliderInspector::onLinearDampingChanged()
	{
		mCollider->linearDamping(meta::Field<physic::ICCollider, 6u>::get(mCollider));
	}

	void WICColliderInspector::onAngularDampingChanged()
	{
		mCollider->angularDamping(meta::Field<physic::ICCollider, 7u>::get(mCollider));
	}

	void WICColliderInspector::onGravityChanged()
	{
		mCollider->useGravity(meta::Field<physic::ICCollider, 8u>::get(mCollider));
	}

	void WICColliderInspector::onFreezeChanged()
	{
		mCollider->freezeFlag(meta::Field<physic::ICCollider, 9u>::get(mCollider));
	}
}