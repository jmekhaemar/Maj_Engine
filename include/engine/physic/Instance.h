#ifndef _PHYSIC_INSTANCE_
#define _PHYSIC_INSTANCE_

#include <unordered_map>

#include <Service.h>

#include "physic/PhysXErrorCallback.h"
#include "physic/PhysXSimulationCallback.h"
#include "physic/Layer.h"
#include "physic/PhysicMaterial.h"

namespace maj::scene
{
	class Actor;
}

namespace maj::physic
{
	class Instance final : public Service
	{
	private:
		static physx::PxFilterFlags filterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1,
												 physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* kConstantBlock, physx::PxU32 constantBlockSize);

		static PhysXErrorCallback sErrorCallback;
		static PhysXSimulationCallback sSimulationCallback;
		static physx::PxDefaultAllocator sDefaultAllocatorCallback;
		static LayerFilter sLayerFilter;
	
	public:
		static LayerFilter& layerFilter();
	
	public:
		Instance();
		~Instance();

		physx::PxRigidActor* createRigidActor(scene::Actor* actor);
		physx::PxShape* createShape(physx::PxRigidActor* rigidActor, const physx::PxGeometry& kGeometry, const PhysicMaterial& kMaterial);
		physx::PxMaterial* createMaterial(float staticFriction, float dynamicFriction, float restitution);

		const physx::PxScene* scene() const;
		physx::PxScene* scene();

	private:
		physx::PxFoundation* mFoundation;
		physx::PxPvd* mPvd;
		physx::PxPhysics* mPhysics;
		physx::PxScene* mScene;
	};
}

#endif