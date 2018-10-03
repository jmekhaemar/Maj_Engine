#ifndef _PHYSX_HELPER_H_
#define _PHYSX_HELPER_H_

#include <PxPhysicsAPI.h>

#include <Transform.h>

namespace maj::physic
{
	physx::PxVec3 convert(const math::Vec3f& kVec) noexcept;
	physx::PxQuat convert(const math::Quatf& kQuat) noexcept;
	physx::PxTransform convert(const math::Transf& kTransf) noexcept;

	math::Vec3f convert(const physx::PxVec3& kVec) noexcept;
	math::Quatf convert(const physx::PxQuat& kQuat) noexcept;
	math::Transf convert(const physx::PxTransform& kTransf) noexcept;
}

#endif