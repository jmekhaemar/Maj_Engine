#include "physic/CBoxCollider.h"

#include "gizmo/Wireframe.h"
#include "scene/SceneHandler.h"
#include "physic/Instance.h"
#include "physic/PhysXHelper.h"

namespace maj::physic
{
	DEFINE_HASH_OVERLOAD(CBoxCollider)

	CBoxCollider::CBoxCollider(scene::Actor* actor, const PhysicMaterial* kMaterial, ELayer layer, bool isTrigger)
		: ICCollider { actor, kMaterial, layer, isTrigger }, mHalfExtent { mActor->rootTransform()->global().scale() * 0.5f }
	{
		initialize();
		onAttach(nullptr);
	}

	CBoxCollider::CBoxCollider(scene::Actor* actor, const math::Vec3f& kHalfExtent, const math::Vec3f& kDeltaPos, const PhysicMaterial* kMaterial, ELayer layer, bool isTrigger)
		: ICCollider { actor, kMaterial, layer, isTrigger }, mHalfExtent { kHalfExtent }
	{
		mDeltaPos = kDeltaPos;

		initialize();
		onAttach(nullptr);
	}

	CBoxCollider::~CBoxCollider()
	{}

	void CBoxCollider::initialize()
	{}

	const math::Vec3f& CBoxCollider::halfExtent() const
	{
		return mHalfExtent;
	}

	void CBoxCollider::halfExtent(const math::Vec3f& kExtent)
	{
		mHalfExtent = kExtent;
		updateShape();
	}

	void CBoxCollider::extent(const math::Vec3f& kExtent)
	{
		mHalfExtent = kExtent * 0.5f;
		updateShape();
	}

	void CBoxCollider::awake()
	{
		ICCollider::awake();

#ifdef MAJ_EDITOR
		gizmo::GizmoHandler* gizmoHandler { Service::get<gizmo::GizmoHandler>() };
		gizmo::Wireframe* wireframe { gizmoHandler->add<gizmo::Wireframe>(this, Asset<resources::Model>::get("asset/default/cube")) };
		wireframe->color({ 0.f, 1.f, 1.f });
		wireframe->transform([this] () -> math::Transf
		{
			math::Transf transform { convert(physx::PxShapeExt::getGlobalPose(*mShape, *mRigidActor)) };
			transform.scale(mHalfExtent * 2.f);
			return transform;
		});
#endif
	}

	void CBoxCollider::onAttach(scene::Actor* previousActor)
	{
		ICCollider::onAttach(previousActor);

		createRigidActor();
	}

	std::unique_ptr<physx::PxGeometry> CBoxCollider::geometry() const
	{
		return std::make_unique<physx::PxBoxGeometry>(convert(mHalfExtent));
	}
}