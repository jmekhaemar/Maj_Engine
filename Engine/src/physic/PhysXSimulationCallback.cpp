#include "physic/PhysXSimulationCallback.h"

#include "scene/Actor.h"
#include "physic/PhysXErrorCallback.h"
#include "physic/PhysXHelper.h"
#include "physic/CollisionData.h"

namespace maj::physic
{
	void PhysXSimulationCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		(void) constraints;
		(void) count;
	}

	void PhysXSimulationCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		(void) actors;
		(void) count;
	}

	void PhysXSimulationCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		(void) actors;
		(void) count;
	}

	void PhysXSimulationCallback::onContact(const physx::PxContactPairHeader& kPairHeader, const physx::PxContactPair* kPairs, physx::PxU32 nbPairs)
	{
		(void) kPairHeader;

		for (physx::PxU32 i { 0u } ; i < nbPairs ; ++i)
		{
			const physx::PxContactPair& kCp { kPairs[i] };

			// If contact was lost with a removed actor
			if (kCp.flags.isSet(physx::PxContactPairFlag::eREMOVED_SHAPE_0) || kCp.flags.isSet(physx::PxContactPairFlag::eREMOVED_SHAPE_1))
				continue;

			scene::Actor* actor0 { static_cast<scene::Actor*>(kCp.shapes[0]->getActor()->userData) };
			scene::Actor* actor1 { static_cast<scene::Actor*>(kCp.shapes[1]->getActor()->userData) };
			if (!actor0 || !actor1)
				continue;

			physx::PxContactPairPoint* contacts { new physx::PxContactPairPoint[kCp.contactStreamSize] };
			physx::PxU32 nbContacts{ kCp.extractContacts(contacts, kCp.contactStreamSize) };

			CollisionData data {};
			data.mCollider = actor0->component<ICCollider>();
			data.mOtherCollider = actor1->component<ICCollider>();

			for (physx::PxU32 j { 0u } ; j < nbContacts ; ++j)
			{
				CollisionPoint point { convert(contacts[j].position), contacts[j].separation, convert(contacts[j].normal), convert(contacts[j].impulse) };
				data.mCollisionPoints.emplace_back(std::move(point));
			}

			delete[] contacts;

			if (kCp.events & physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND)
			{
				actor0->mOnCollisionEnter(&data);
				actor1->mOnCollisionEnter(&data);
			}
			else if (kCp.events & physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS)
			{
				actor0->mOnCollisionStay(&data);
				actor1->mOnCollisionStay(&data);
			}
			else if (kCp.events & physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST)
			{
				actor0->mOnCollisionExit(&data);
				actor1->mOnCollisionExit(&data);
			}
		}
	}

	void PhysXSimulationCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		for (physx::PxU32 i { 0u } ; i < count ; ++i)
		{
			const physx::PxTriggerPair& kCp { pairs[i] };
			scene::Actor* triggerActor { static_cast<scene::Actor*>(kCp.triggerActor->userData) };
			scene::Actor* otherActor { static_cast<scene::Actor*>(kCp.otherActor->userData) };

			if (kCp.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
				triggerActor->mOnTriggerEnter(static_cast<scene::Actor*&&>(otherActor));
			else if (kCp.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
				triggerActor->mOnTriggerExit(static_cast<scene::Actor*&&>(otherActor));
		}
	}

	void PhysXSimulationCallback::onAdvance(const physx::PxRigidBody* const* kBodyBuffer, const physx::PxTransform* kPoseBuffer, const physx::PxU32 kCount)
	{
		for (physx::PxU32 i { 0u } ; i < kCount ; ++i)
		{
			scene::Actor* actor { static_cast<scene::Actor*>(kBodyBuffer[i]->userData) };
			actor->rootTransform()->position(convert(kPoseBuffer[i].p));
			actor->rootTransform()->rotation(convert(kPoseBuffer[i].q));
		}
	}
}