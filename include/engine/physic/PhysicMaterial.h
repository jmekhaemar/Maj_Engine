#ifndef _PHYSIC_MATERIAL_H_
#define _PHYSIC_MATERIAL_H_

#include <PxPhysicsAPI.h>

#include <Asset.h>

namespace maj::physic
{
	class PhysicMaterial final : public ISerializable
	{
		ENABLE_META(PhysicMaterial)

	public:
		LOAD_CONSTRUCTOR(PhysicMaterial)

		PhysicMaterial(float staticFriction = 0.1f , float dynamicFriction = 0.5f, float restitution = 0.5f);
		~PhysicMaterial();

		const physx::PxMaterial& material() const;

		float staticFriction() const;
		float dynamicFriction() const;
		float restitution() const;

		Event<void> mOnChange;

	private:
		physx::PxMaterial* mMaterial;

		float mStaticFriction;
		float mDynamicFriction;
		float mRestitution;
	};
}

META_CLASS(maj::Asset<maj::physic::PhysicMaterial>, 1u)
META_FIELD(maj::Asset<maj::physic::PhysicMaterial>, uint32_t, mGuid, 0u)

META_CLASS(maj::physic::PhysicMaterial, 4u)
META_FIELD(maj::physic::PhysicMaterial, uint32_t, mGuid, 0u)
META_FIELD(maj::physic::PhysicMaterial, float, mStaticFriction, 1u)
META_FIELD(maj::physic::PhysicMaterial, float, mDynamicFriction, 2u)
META_FIELD(maj::physic::PhysicMaterial, float, mRestitution, 3u)

#endif