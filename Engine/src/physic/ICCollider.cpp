#include "physic/ICCollider.h"

#include <Renderer.h>

#include "scene/SceneHandler.h"
#include "scene/CTransform.h"
#include "scene/CCamera.h"
#include "physic/Instance.h"
#include "physic/PhysXHelper.h"

namespace maj::physic
{
	DEFINE_HASH_OVERLOAD(ICCollider)

	ICCollider::ICCollider()
		: IComponent {}, mMaterial {}, mRigidActor { nullptr }, mShape { nullptr }, mDeltaPos {}, mDeltaRot {}, mLayer { 0u }, mIsTrigger { false }, mMass { 1.f },
		mLinearDamping { 1.f }, mAngularDamping { 0.1f }, mUseGravity { true }, mFreeze { EFreezeFlag::NONE }, mStaticEventIndex { 0u }, mTransfChangedEvent { 0u }
	{}

	ICCollider::ICCollider(scene::Actor* actor, const PhysicMaterial* kMaterial, ELayer layer, bool isTrigger)
		: IComponent { actor }, mMaterial {}, mRigidActor { nullptr }, mShape { nullptr }, mDeltaPos {}, mDeltaRot {}, mLayer { layer }, mIsTrigger { isTrigger },
		mMass { 1.f }, mLinearDamping { 1.f }, mAngularDamping { 0.1f }, mUseGravity { true }, mFreeze { EFreezeFlag::NONE }, mStaticEventIndex { 0u }, mTransfChangedEvent { 0u }
	{
		if (kMaterial)
			mMaterial = kMaterial;

		initialize();
	}

	ICCollider::~ICCollider()
	{}

	void ICCollider::initialize()
	{
		// Enable/Disable
		mOnEnabled += [&] () -> void
		{
			bool isStatic { false };
			if (mActor)
			{
				scene::CTransform* rootTr { mActor->rootTransform() };
				if (!rootTr)
				{
					LOG(LogPhysicWarning, "failed to enable collider: actor \'" << mActor->name() << "\' has no root transform")
					return;
				}

				isStatic = rootTr->isStatic();
				mStaticEventIndex = rootTr->mOnStaticChanged + std::bind(&ICCollider::createRigidActor, this);
				mTransfChangedEvent = rootTr->mOnChanged + std::bind(&ICCollider::synchronize, this);
			}

			createRigidActor();

#ifdef MAJ_EDITOR
			Service::get<gizmo::GizmoHandler>()->enable(this, true);
#endif
		};

		mOnDisabled += [&](bool destroyed) -> void
		{
			bool isStatic { false };
			if (mActor)
			{
				scene::CTransform* rootTr { mActor->rootTransform() };
				if (rootTr)
				{
					rootTr->mOnStaticChanged -= mStaticEventIndex;
					rootTr->mOnChanged -= mTransfChangedEvent;
					isStatic = rootTr->isStatic();
				}
			}

			if (mRigidActor)
			{
				Service::get<Instance>()->scene()->removeActor(*mRigidActor);

				mRigidActor->release();
				mRigidActor = nullptr;
				mShape = nullptr;
			}

#ifdef MAJ_EDITOR
			if (destroyed)
				Service::get<gizmo::GizmoHandler>()->remove(this);
			else
				Service::get<gizmo::GizmoHandler>()->enable(this, false);
#endif
		};

		// Material
		if (mMaterial.empty())
			mMaterial = Asset<physic::PhysicMaterial>::get("asset/default/physicMaterial");

		mMaterial.onChange([this]()->void
		{
			physx::PxMaterial* const mat { const_cast<physx::PxMaterial* const>(&mMaterial->material()) };
			mShape->setMaterials(&mat, 1u);
		});
	}

	const math::Vec3f& ICCollider::deltaPosition() const
	{
		return mDeltaPos;
	}

	void ICCollider::deltaPosition(const math::Vec3f& kDeltaPos)
	{
		mDeltaPos = kDeltaPos;

		if (mShape)
		{
			math::Transf tr { mDeltaPos, mDeltaRot };
			mShape->setLocalPose(convert(tr));
		}

		if (mRigidActor && !mActor->rootTransform()->isStatic())
			static_cast<physx::PxRigidBody*>(mRigidActor)->setCMassLocalPose(mShape->getLocalPose());
	}

	const math::Quatf& ICCollider::deltaRotation() const
	{
		return mDeltaRot;
	}

	void ICCollider::deltaRotation(const math::Quatf& kDeltaRot)
	{
		mDeltaRot = kDeltaRot;

		if (mShape)
		{
			math::Transf tr { mDeltaPos, mDeltaRot };
			mShape->setLocalPose(convert(tr));
		}

		if (mRigidActor && !mActor->rootTransform()->isStatic())
			static_cast<physx::PxRigidBody*>(mRigidActor)->setCMassLocalPose(mShape->getLocalPose());
	}

	ELayer ICCollider::layer() const
	{
		return mLayer;
	}

	void ICCollider::layer(ELayer layer)
	{
		mLayer = layer;

		if (mShape)
		{
			physx::PxFilterData filterData {};
			filterData.word0 = static_cast<int32_t>(mLayer);

			mShape->setSimulationFilterData(filterData);
			mShape->setQueryFilterData(filterData);
		}
	}

	bool ICCollider::isTrigger() const
	{
		return mIsTrigger;
	}

	void ICCollider::isTrigger(bool trigger)
	{
		mIsTrigger = trigger;

		if (!mShape)
			return;

		if (mIsTrigger)
		{
			mShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !mIsTrigger);
			mShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, mIsTrigger);
		}
		else
		{
			mShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, mIsTrigger);
			mShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !mIsTrigger);
		}
	}

	math::Vec3f ICCollider::linearVelocity() const
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return {};

		return convert(static_cast<physx::PxRigidDynamic*>(mRigidActor)->getLinearVelocity());
	}

	void ICCollider::linearVelocity(const math::Vec3f& velocity)
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return;

		static_cast<physx::PxRigidDynamic*>(mRigidActor)->setLinearVelocity(convert(velocity));
	}

	math::Vec3f ICCollider::angularVelocity() const
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return {};

		return convert(static_cast<physx::PxRigidDynamic*>(mRigidActor)->getAngularVelocity());
	}

	void ICCollider::angularVelocity(const math::Vec3f& velocity)
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return;

		static_cast<physx::PxRigidDynamic*>(mRigidActor)->setAngularVelocity(convert(velocity));
	}

	float ICCollider::mass() const
	{
		return mMass;
	}

	void ICCollider::mass(float mass)
	{
		mMass = mass;

		if (mRigidActor && !mActor->rootTransform()->isStatic())
			static_cast<physx::PxRigidDynamic*>(mRigidActor)->setMass(mass);
	}

	float ICCollider::linearDamping() const
	{
		return mLinearDamping;
	}

	void ICCollider::linearDamping(float damping)
	{
		mLinearDamping = damping;

		if (mRigidActor && !mActor->rootTransform()->isStatic())
			static_cast<physx::PxRigidDynamic*>(mRigidActor)->setLinearDamping(damping);
	}

	float ICCollider::angularDamping() const
	{
		return mAngularDamping;
	}

	void ICCollider::angularDamping(float damping)
	{
		mAngularDamping = damping;

		if (mRigidActor && !mActor->rootTransform()->isStatic())
			static_cast<physx::PxRigidDynamic*>(mRigidActor)->setAngularDamping(damping);
	}

	bool ICCollider::useGravity() const
	{
		return mUseGravity;
	}

	void ICCollider::useGravity(bool use)
	{
		mUseGravity = use;

		if (mRigidActor && !mActor->rootTransform()->isStatic())
			static_cast<physx::PxRigidDynamic*>(mRigidActor)->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !mUseGravity);
	}

	EFreezeFlag ICCollider::freezeFlag() const
	{
		return mFreeze;
	}

	void ICCollider::freezeFlag(EFreezeFlag freeze)
	{
		mFreeze = freeze;

		if (mRigidActor && !mActor->rootTransform()->isStatic())
			static_cast<physx::PxRigidDynamic*>(mRigidActor)->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlag::Enum>(freeze));
	}

	void ICCollider::addForce(const math::Vec3f& kForce, EForceMode mode)
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return;

		static_cast<physx::PxRigidDynamic*>(mRigidActor)->addForce(convert(kForce), static_cast<physx::PxForceMode::Enum>(mode));
	}

	void ICCollider::addTorque(const math::Vec3f& kTorque, EForceMode mode)
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return;

		static_cast<physx::PxRigidDynamic*>(mRigidActor)->addTorque(convert(kTorque), static_cast<physx::PxForceMode::Enum>(mode));
	}

	void ICCollider::clearForce(EForceMode mode)
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return;

		static_cast<physx::PxRigidDynamic*>(mRigidActor)->clearForce(static_cast<physx::PxForceMode::Enum>(mode));
	}

	void ICCollider::clearTorque(EForceMode mode)
	{
		if (!mRigidActor || mActor->rootTransform()->isStatic())
			return;

		static_cast<physx::PxRigidDynamic*>(mRigidActor)->clearTorque(static_cast<physx::PxForceMode::Enum>(mode));
	}

	void ICCollider::synchronize()
	{
		if (!mRigidActor)
			return;

		mRigidActor->setGlobalPose(convert(mActor->rootTransform()->global()));
	}

	void ICCollider::onAttach(scene::Actor* previousActor)
	{
		if (previousActor)
		{
			scene::CTransform* rootTr { previousActor->rootTransform() };
			if (rootTr)
			{
				rootTr->mOnStaticChanged -= mStaticEventIndex;
				rootTr->mOnChanged -= mTransfChangedEvent;
			}
		}

		scene::CTransform* rootTr { mActor->rootTransform() };
		if (mIsEnabled)
		{
			if (!rootTr)
			{
				LOG(LogPhysicWarning, "failed to attach collider: actor \'" << mActor->name() << "\' has no root transform")
				enable(false);
				return;
			}

			mStaticEventIndex = rootTr->mOnStaticChanged + std::bind(&ICCollider::createRigidActor, this);
			mTransfChangedEvent = rootTr->mOnChanged + std::bind(&ICCollider::synchronize, this);
		}
	}

	void ICCollider::createRigidActor()
	{
		physic::Instance* instance { Service::get<Instance>() };

		if (mRigidActor)
		{
			Service::get<Instance>()->scene()->removeActor(*mRigidActor);

			mRigidActor->release();
			mRigidActor = nullptr;
			mShape = nullptr;
		}

		mRigidActor = instance->createRigidActor(mActor);

		mass(mMass);
		linearDamping(mLinearDamping);
		angularDamping(mAngularDamping);
		useGravity(mUseGravity);
		freezeFlag(mFreeze);

		mShape = instance->createShape(mRigidActor, *geometry(), *mMaterial);

		isTrigger(mIsTrigger);
		layer(mLayer);
		deltaPosition(mDeltaPos);
	}

	void ICCollider::updateShape()
	{
		if (!mRigidActor || !mShape)
			return;

		mShape->setGeometry(*geometry());
	}
}