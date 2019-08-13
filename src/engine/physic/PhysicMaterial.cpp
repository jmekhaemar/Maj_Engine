#include "physic/PhysicMaterial.h"

#include "physic/Instance.h"

namespace maj::physic
{
	DEFINE_HASH_OVERLOAD(PhysicMaterial)

	PhysicMaterial::PhysicMaterial(float staticFriction, float dynamicFriction, float restitution)
		: mMaterial { nullptr }, mStaticFriction { staticFriction }, mDynamicFriction { dynamicFriction }, mRestitution { restitution }
	{
		initialize();
	}

	PhysicMaterial::~PhysicMaterial()
	{
		// PhysX material is destroyed with the physx instance
	}

	void PhysicMaterial::initialize()
	{
		physic::Instance* instance { Service::get<Instance>() };
		mMaterial = instance->createMaterial(mStaticFriction, mDynamicFriction, mRestitution);
	}

	const physx::PxMaterial& PhysicMaterial::material() const
	{
		return *mMaterial;
	}

	float PhysicMaterial::staticFriction() const
	{
		return mStaticFriction;
	}

	float PhysicMaterial::dynamicFriction() const
	{
		return mDynamicFriction;
	}

	float PhysicMaterial::restitution() const
	{
		return mRestitution;
	}
}