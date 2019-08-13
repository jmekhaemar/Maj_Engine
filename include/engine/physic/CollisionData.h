#ifndef _COLLISION_DATA_H_
#define _COLLISION_DATA_H_

#include <vector>

#include "physic/ICCollider.h"

namespace maj::physic
{
	struct CollisionPoint final
	{
		math::Vec3f mPosition;
		float mSeparation;
		math::Vec3f mNormal;
		math::Vec3f mImpulse;
	};

	struct CollisionData final
	{
		std::vector<CollisionPoint> mCollisionPoints;
		ICCollider* mCollider;
		ICCollider* mOtherCollider;
	};
}

#endif