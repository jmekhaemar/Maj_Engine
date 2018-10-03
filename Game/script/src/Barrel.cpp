#include "Barrel.h"

#include <ww/Input.h>

namespace maj::script
{
	DEFINE_HASH_OVERLOAD(Barrel)

	Barrel::Barrel(JsonNode* root)
		: scene::CScript {}, mCollider { nullptr }, mColliderDisabled { 0u }
	{
		load(root);
		initialize();
	}

	Barrel::Barrel(scene::Actor* actor)
		: scene::CScript { actor }, mCollider { nullptr }, mColliderDisabled { 0u }
	{
		initialize();
	}

	Barrel::~Barrel()
	{
		if (mCollider)
			mCollider->mOnDisabled -= mColliderDisabled;
	}

	void Barrel::initialize()
	{}

	void Barrel::awake()
	{
		CScript::awake();
		getCollider();
	}

	void Barrel::update(float deltaTime)
	{
		(void)deltaTime;

		scene::CTransform* actorTr { mActor->rootTransform() };

		const ww::Input* kInput { Service::get<ww::Input>() };
		if (kInput->key(ww::EKey::PAD_SUBTRACT).isPressed())
		{
			actorTr->scale(actorTr->scale() * 0.5f);
			
			if (mCollider)
			{
				mCollider->synchronize();
				mCollider->radius(mCollider->radius() * 0.5f);
				mCollider->deltaPosition(mCollider->deltaPosition() * 0.5f);
			}
		}

		if (kInput->key(ww::EKey::PAD_ADD).isPressed())
		{
			actorTr->scale(actorTr->scale() * 2.f);

			if (mCollider)
			{
				mCollider->synchronize();
				mCollider->radius(mCollider->radius() * 2.f);
				mCollider->deltaPosition(mCollider->deltaPosition() * 2.f);
			}
		}

		if (kInput->key(ww::EKey::PAD_ENTER).isPressed() && mCollider)
		{
			if (mCollider->useGravity())
			{
				mCollider->useGravity(false);
				mCollider->mass(10.f);
				mCollider->linearDamping(0.2f);
				mCollider->freezeFlag(physic::EFreezeFlag::ANGULAR_Z);

				mCollider->addForce({ 2.f, 3.f, -2.f }, physic::EForceMode::IMPULSE);
				mCollider->addTorque({ 0.f, 0.5f, 0.f }, physic::EForceMode::IMPULSE);
			}
			else
			{
				mCollider->useGravity(true);
				mCollider->linearDamping(1.f);
				mCollider->mass(15.f);
				mCollider->freezeFlag(maj::physic::EFreezeFlag::NONE);

				mCollider->clearForce();
				mCollider->clearTorque();
			}
		}

		if (kInput->key(ww::EKey::PAD_3).isPressed() && mCollider)
			mCollider->enable(!mCollider->isEnabled());
	}

	void Barrel::getCollider()
	{
		mCollider = mActor->component<physic::CSphereCollider>();
		if (mCollider)
		{
			mColliderDisabled = mCollider->mOnDisabled + [this] (bool destroyed) -> void
			{
				if (destroyed)
					getCollider();
			};
		}
	}
}