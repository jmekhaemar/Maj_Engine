#ifndef _CCAPSULE_COLLIDER_H_
#define _CCAPSULE_COLLIDER_H_

#include "physic/ICCollider.h"

namespace maj::physic
{
	class CCapsuleCollider final : public ICCollider
	{
		ENABLE_META_PARENT(CCapsuleCollider, ICCollider)

	public:
		LOAD_CONSTRUCTOR(CCapsuleCollider)

		CCapsuleCollider(scene::Actor* actor, const PhysicMaterial* kMaterial = nullptr, ELayer layer = ELayer::LAYER_1, bool isTrigger = false);
		CCapsuleCollider(scene::Actor* actor, float radius, float halfHeight, const math::Vec3f& kDeltaPos = {}, const PhysicMaterial* kMaterial = nullptr,
						 ELayer layer = ELayer::LAYER_1, bool isTrigger = false);
		virtual ~CCapsuleCollider();

		float radius() const;
		void radius(float radius);

		float halfHeight() const;
		void halfHeight(float halfHeight);
		void height(float height);

		virtual void awake() override;

	private:
		virtual void onAttach(scene::Actor* previousActor) override;

		virtual std::unique_ptr<physx::PxGeometry> geometry() const override;

		float mRadius;
		float mHalfHeight;
	};
}

META_CLASS_PARENT(maj::physic::CCapsuleCollider, maj::physic::ICCollider, 2u)
META_FIELD(maj::physic::CCapsuleCollider, float, mRadius, 0u)
META_FIELD(maj::physic::CCapsuleCollider, float, mHalfHeight, 1u)

#endif