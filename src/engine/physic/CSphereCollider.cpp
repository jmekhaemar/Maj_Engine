#include "physic/CSphereCollider.h"

#include "gizmo/Wireframe.h"
#include "scene/SceneHandler.h"
#include "physic/PhysXHelper.h"

namespace maj::physic
{
	DEFINE_HASH_OVERLOAD(CSphereCollider)

	CSphereCollider::CSphereCollider(scene::Actor* actor, const PhysicMaterial* kMaterial, ELayer layer, bool isTrigger)
		: ICCollider { actor, kMaterial, layer, isTrigger }, mRadius { mActor->rootTransform()->global().scale().mX * 0.5f }
	{
		initialize();
		onAttach(nullptr);
	}

	CSphereCollider::CSphereCollider(scene::Actor* actor, float radius, const math::Vec3f& kDeltaPos, const PhysicMaterial* kMaterial, ELayer layer, bool isTrigger)
		: ICCollider { actor, kMaterial, layer, isTrigger }, mRadius{ radius }
	{
		mDeltaPos = kDeltaPos;

		initialize();
		onAttach(nullptr);
	}

	CSphereCollider::~CSphereCollider()
	{}

	void CSphereCollider::initialize()
	{}

	float CSphereCollider::radius() const
	{
		return mRadius;
	}

	void CSphereCollider::radius(float radius)
	{
		mRadius = radius;
		updateShape();
	}

	void CSphereCollider::awake()
	{
		ICCollider::awake();

#ifdef MAJ_EDITOR
		gizmo::GizmoHandler* gizmoHandler { Service::get<gizmo::GizmoHandler>() };
		gizmo::Wireframe* wireframe { gizmoHandler->add<gizmo::Wireframe>(this, Asset<resources::Model>::get("asset/default/sphere")) };
		wireframe->color({ 0.f, 1.f, 0.f });
		wireframe->transform([this] () -> math::Transf
		{
			math::Transf transform { convert(physx::PxShapeExt::getGlobalPose(*mShape, *mRigidActor)) };
			transform.scale(math::Vec3f { mRadius } * 1.45f);
			return transform;
		});
#endif
	}

	void CSphereCollider::onAttach(scene::Actor* previousActor)
	{
		ICCollider::onAttach(previousActor);

		createRigidActor();
	}

	std::unique_ptr<physx::PxGeometry>  CSphereCollider::geometry() const
	{
		return std::make_unique<physx::PxSphereGeometry>(mRadius);
	}
}