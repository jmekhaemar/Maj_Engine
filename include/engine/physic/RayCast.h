#ifndef _RAY_CAST_H_
#define _RAY_CAST_H_

#include "physic/Instance.h"
#include "physic/ICCollider.h"

namespace maj::physic
{
	struct RayCastHit final
	{
		ICCollider* mCollider;
		math::Vec3f mPosition;
		math::Vec3f mNormal;
	};

	class RayCast final
	{
	public:
		RayCast(const math::Vec3f& kOrigin, const math::Vec3f& kDirection, float maxDistance, ELayer layerMask = ELayer::LAYER_1);
		~RayCast() = default;

		bool operator()(RayCastHit& hit);

	private:
		math::Vec3f mOrigin;
		math::Vec3f mDirection;
		float mMaxDistance;
		ELayer mLayerMask;
	};
}

#endif