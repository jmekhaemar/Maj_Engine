#ifndef _PHYSX_SIMULATION_CALLBACK_H_
#define _PHYSX_SIMULATION_CALLBACK_H_

#include <PxPhysicsAPI.h>

namespace maj::physic
{
	class PhysXSimulationCallback final : public physx::PxSimulationEventCallback
	{
	public:
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;

		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override;

		virtual void onContact(const physx::PxContactPairHeader& kPairHeader, const physx::PxContactPair* kPairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onAdvance(const physx::PxRigidBody* const* kBodyBuffer, const physx::PxTransform* kPoseBuffer, const physx::PxU32 kCount) override;
	};
}

#endif