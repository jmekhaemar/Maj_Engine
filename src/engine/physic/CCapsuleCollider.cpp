#include "physic/CCapsuleCollider.h"

#include "gizmo/Wireframe.h"
#include "scene/SceneHandler.h"
#include "physic/PhysXHelper.h"

namespace maj::physic
{
	DEFINE_HASH_OVERLOAD(CCapsuleCollider)

	CCapsuleCollider::CCapsuleCollider(scene::Actor* actor, const PhysicMaterial* kMaterial, ELayer layer, bool isTrigger)
		: ICCollider { actor, kMaterial, layer, isTrigger }, mRadius { 0.25f }, mHalfHeight { mActor->rootTransform()->global().scale().mX * 0.5f }
	{
		mDeltaRot = math::Quatf { 90.f, { 0.f, 0.f, 1.f } };

		initialize();
		onAttach(nullptr);
	}

	CCapsuleCollider::CCapsuleCollider(scene::Actor* actor, float radius, float halfHeight, const math::Vec3f& kDeltaPos, const PhysicMaterial* kMaterial, ELayer layer, bool isTrigger)
		: ICCollider { actor, kMaterial, layer, isTrigger }, mRadius{ radius }, mHalfHeight { halfHeight }
	{
		mDeltaPos = kDeltaPos;
		mDeltaRot = math::Quatf { 90.f, { 0.f, 0.f, 1.f } };

		initialize();
		onAttach(nullptr);
	}

	CCapsuleCollider::~CCapsuleCollider()
	{}

	void CCapsuleCollider::initialize()
	{}

	float CCapsuleCollider::radius() const
	{
		return mRadius;
	}

	void CCapsuleCollider::radius(float radius)
	{
		mRadius = radius;
		updateShape();
	}

	float CCapsuleCollider::halfHeight() const
	{
		return mHalfHeight;
	}

	void CCapsuleCollider::halfHeight(float halfHeight)
	{
		mHalfHeight = halfHeight;
		updateShape();
	}

	void CCapsuleCollider::height(float height)
	{
		mHalfHeight = height * 0.5f;
		updateShape();
	}

	void CCapsuleCollider::awake()
	{
		ICCollider::awake();

#ifdef MAJ_EDITOR
		gizmo::GizmoHandler* gizmoHandler { Service::get<gizmo::GizmoHandler>() };
		gizmo::Wireframe* wireframe { gizmoHandler->add<gizmo::Wireframe>(this, Asset<resources::Model>::get("asset/default/cube")) };
		wireframe->color({ 1.f, 0.f, 0.f });
		wireframe->transform([this] () -> math::Transf
		{
			math::Transf transform { convert(physx::PxShapeExt::getGlobalPose(*mShape, *mRigidActor)) };
			transform.scale(math::Vec3f { mHalfHeight + mRadius, mRadius, mRadius } *2.f);
			return transform;
		});
#endif
	}

	void CCapsuleCollider::onAttach(scene::Actor* previousActor)
	{
		ICCollider::onAttach(previousActor);

		if (mIsEnabled)
			createRigidActor();
	}

	std::unique_ptr<physx::PxGeometry> CCapsuleCollider::geometry() const
	{
		return std::make_unique<physx::PxCapsuleGeometry>(mRadius, mHalfHeight);
	}
}