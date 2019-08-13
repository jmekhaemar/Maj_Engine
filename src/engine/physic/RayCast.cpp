#include "physic/RayCast.h"

#include "scene/Actor.h"
#include "physic/PhysXHelper.h"

namespace maj::physic
{
	RayCast::RayCast(const math::Vec3f& kOrigin, const math::Vec3f& kDirection, float maxDistance, ELayer layerMask)
		: mOrigin { kOrigin }, mDirection { kDirection }, mMaxDistance { maxDistance }, mLayerMask { layerMask }
	{}

	bool RayCast::operator()(RayCastHit& hit)
	{
		physx::PxRaycastBuffer physxHit {};
		physx::PxQueryFilterData filterData {};
		filterData.data.word0 = static_cast<int32_t>(mLayerMask);

		bool status { Service::get<Instance>()->scene()->raycast(convert(mOrigin), convert(mDirection), mMaxDistance, physxHit, physx::PxHitFlag::eDEFAULT, filterData) };
		if (status)
		{
			hit.mCollider = static_cast<scene::Actor*>(physxHit.block.actor->userData)->component<ICCollider>();
			hit.mPosition = convert(physxHit.block.position);
			hit.mNormal = convert(physxHit.block.normal);
		}

		return status;
	}
}