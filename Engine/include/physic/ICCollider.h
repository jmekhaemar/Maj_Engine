#ifndef _ICCOLIDER_H_
#define _ICCOLIDER_H_

#include <Flag.h>
#include <Vec3.h>
#include <Quaternion.h>

#include "scene/IComponent.h"
#include "physic/Instance.h"

namespace maj::physic
{
	enum class EFreezeFlag : uint8_t
	{
		NONE = 0u,
		LINEAR_X = 1u,
		LINEAR_Y = 2u,
		LINEAR_Z = 4u,
		ANGULAR_X = 8u,
		ANGULAR_Y = 16u,
		ANGULAR_Z = 32u
	};

	enum class EForceMode : uint8_t
	{
		FORCE = physx::PxForceMode::eFORCE,
		IMPULSE = physx::PxForceMode::eIMPULSE,
		VELOCITY_CHANGE = physx::PxForceMode::eVELOCITY_CHANGE,
		ACCELERATION = physx::PxForceMode::eACCELERATION
	};

	DECLARE_FLAG(EFreezeFlag)

	class ICCollider : public scene::IComponent
	{
		ENABLE_META_PARENT(ICCollider, scene::IComponent)

	public:
		ICCollider(scene::Actor* actor, const PhysicMaterial* kMaterial = nullptr, ELayer layer = ELayer::LAYER_1, bool isTrigger = false);
		virtual ~ICCollider();

		INITIALIZE_SIGNATURE();

		const math::Vec3f& deltaPosition() const;
		void deltaPosition(const math::Vec3f& kDeltaPos);

		const math::Quatf& deltaRotation() const;
		void deltaRotation(const math::Quatf& kDeltaRot);

		ELayer layer() const;
		void layer(ELayer layer);

		bool isTrigger() const;
		void isTrigger(bool trigger);

		math::Vec3f linearVelocity() const;
		void linearVelocity(const math::Vec3f& velocity);

		math::Vec3f angularVelocity() const;
		void angularVelocity(const math::Vec3f& velocity);

		float mass() const;
		void mass(float mass);

		float linearDamping() const;
		void linearDamping(float damping);

		float angularDamping() const;
		void angularDamping(float damping);

		bool useGravity() const;
		void useGravity(bool use);

		EFreezeFlag freezeFlag() const;
		void freezeFlag(EFreezeFlag freeze);

		void addForce(const math::Vec3f& kForce, EForceMode mode = EForceMode::FORCE);
		void addTorque(const math::Vec3f& kTorque, EForceMode mode = EForceMode::FORCE);

		void clearForce(EForceMode mode = EForceMode::FORCE);
		void clearTorque(EForceMode mode = EForceMode::FORCE);

		void synchronize();

	protected:
		ICCollider();

		virtual void onAttach(scene::Actor* previousActor) override = 0;

		virtual std::unique_ptr<physx::PxGeometry> geometry() const = 0;
		void createRigidActor();
		void updateShape();

		Asset<PhysicMaterial> mMaterial;
		physx::PxRigidActor* mRigidActor;

		physx::PxShape* mShape;
		math::Vec3f mDeltaPos;
		math::Quatf mDeltaRot;

		ELayer mLayer;
		bool mIsTrigger;

		float mMass;
		float mLinearDamping;
		float mAngularDamping;
		bool mUseGravity;
		EFreezeFlag mFreeze;

		uint32_t mStaticEventIndex;
		uint32_t mTransfChangedEvent;
	};
}

#include "physic/ICCollider.inl"

META_FLAG(maj::physic::EFreezeFlag, 7u)
META_FLAG_VALUE(maj::physic::EFreezeFlag, maj::physic::EFreezeFlag::NONE, 0u)
META_FLAG_VALUE(maj::physic::EFreezeFlag, maj::physic::EFreezeFlag::LINEAR_X, 1u)
META_FLAG_VALUE(maj::physic::EFreezeFlag, maj::physic::EFreezeFlag::LINEAR_Y, 2u)
META_FLAG_VALUE(maj::physic::EFreezeFlag, maj::physic::EFreezeFlag::LINEAR_Z, 3u)
META_FLAG_VALUE(maj::physic::EFreezeFlag, maj::physic::EFreezeFlag::ANGULAR_X, 4u)
META_FLAG_VALUE(maj::physic::EFreezeFlag, maj::physic::EFreezeFlag::ANGULAR_Y, 5u)
META_FLAG_VALUE(maj::physic::EFreezeFlag, maj::physic::EFreezeFlag::ANGULAR_Z, 6u)

META_CLASS_PARENT(maj::physic::ICCollider, maj::scene::IComponent, 10u)
META_FIELD(maj::physic::ICCollider, maj::Asset<maj::physic::PhysicMaterial>, mMaterial, 0u)
META_FIELD(maj::physic::ICCollider, maj::physic::ELayer, mLayer, 1u)
META_FIELD(maj::physic::ICCollider, bool, mIsTrigger, 2u)
META_FIELD(maj::physic::ICCollider, maj::math::Vec3f, mDeltaPos, 3u)
META_FIELD(maj::physic::ICCollider, maj::math::Quatf, mDeltaRot, 4u)
META_FIELD(maj::physic::ICCollider, float, mMass, 5u)
META_FIELD(maj::physic::ICCollider, float, mLinearDamping, 6u)
META_FIELD(maj::physic::ICCollider, float, mAngularDamping, 7u)
META_FIELD(maj::physic::ICCollider, bool, mUseGravity, 8u)
META_FIELD(maj::physic::ICCollider, maj::physic::EFreezeFlag, mFreeze, 9u)

#endif