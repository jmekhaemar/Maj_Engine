#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

#include "physic/ICCollider.h"

namespace maj::physic
{
	class CBoxCollider final : public ICCollider
	{
		ENABLE_META_PARENT(CBoxCollider, ICCollider)

	public:
		LOAD_CONSTRUCTOR(CBoxCollider)

		CBoxCollider(scene::Actor* actor, const PhysicMaterial* kMaterial = nullptr, ELayer layer = ELayer::LAYER_1, bool isTrigger = false);
		CBoxCollider(scene::Actor* actor, const math::Vec3f& kHalfExtent, const math::Vec3f& kDeltaPos = {}, const PhysicMaterial* kMaterial = nullptr, ELayer layer = ELayer::LAYER_1,
					 bool isTrigger = false);
		virtual ~CBoxCollider();

		const math::Vec3f& halfExtent() const;
		void halfExtent(const math::Vec3f& kExtent);
		void extent(const math::Vec3f& kExtent);

		virtual void awake() override;

	private:
		virtual void onAttach(scene::Actor* previousActor) override;

		virtual std::unique_ptr<physx::PxGeometry> geometry() const override;

		math::Vec3f mHalfExtent;
	};
}

META_CLASS_PARENT(maj::physic::CBoxCollider, maj::physic::ICCollider, 1u)
META_FIELD(maj::physic::CBoxCollider, maj::math::Vec3f, mHalfExtent, 0u)

#endif