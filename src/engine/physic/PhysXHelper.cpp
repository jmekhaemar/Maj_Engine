#include "physic/PhysXHelper.h"

namespace maj::physic
{
	physx::PxVec3 convert(const math::Vec3f& kVec) noexcept
	{
		return { kVec.mX, kVec.mY, kVec.mZ };
	}

	physx::PxQuat convert(const math::Quatf& kQuat) noexcept
	{
		return { kQuat.mX, kQuat.mY, kQuat.mZ, kQuat.mW };
	}

	physx::PxTransform convert(const math::Transf& kTransf) noexcept
	{
		return { convert(kTransf.position()), convert(kTransf.rotation()) };
	}

	math::Vec3f convert(const physx::PxVec3& kVec) noexcept
	{
		return { kVec.x, kVec.y, kVec.z};
	}

	math::Quatf convert(const physx::PxQuat& kQuat) noexcept
	{
		return { kQuat.w, kQuat.x, kQuat.y, kQuat.z };
	}

	math::Transf convert(const physx::PxTransform& kTransf) noexcept
	{
		return { convert(kTransf.p), convert(kTransf.q) };
	}
}