#ifndef _CSPHERE_COLLIDER_H_
#define _CSPHERE_COLLIDER_H_

#include "physic/ICCollider.h"

namespace maj::physic
{
	class CSphereCollider final : public ICCollider
	{
		ENABLE_META_PARENT(CSphereCollider, ICCollider)

	public:
		LOAD_CONSTRUCTOR(CSphereCollider)

		CSphereCollider(scene::Actor* actor, const PhysicMaterial* kMaterial = nullptr, ELayer layer = ELayer::LAYER_1, bool isTrigger = false);
		CSphereCollider(scene::Actor* actor, float radius, const math::Vec3f& kDeltaPos = {}, const PhysicMaterial* kMaterial = nullptr, ELayer layer = ELayer::LAYER_1,
						bool isTrigger = false);
		virtual ~CSphereCollider();

		float radius() const;
		void radius(float radius);

		virtual void awake() override;

	private:
		virtual void onAttach(scene::Actor* previousActor) override;

		virtual std::unique_ptr<physx::PxGeometry> geometry() const override;

		float mRadius;
	};
}

META_CLASS_PARENT(maj::physic::CSphereCollider, maj::physic::ICCollider, 1u)
META_FIELD(maj::physic::CSphereCollider, float, mRadius, 0u)

#endif