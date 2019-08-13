#include "Player.h"

#include <Input.h>

#include <anim/AnimationInstance.h>
#include <scene/CTransform.h>
#include <scene/SceneHandler.h>
#include <physic/ICCollider.h>

namespace maj::script
{
	DEFINE_HASH_OVERLOAD(Player)

	Player::Player(JsonNode* root)
		: scene::CScript{}, mIsWalking{ false }, mJump{ false }
	{
		load(root);
		initialize();
	}

	Player::Player(scene::Actor* actor)
		: scene::CScript{ actor }, mIsWalking{ false }, mSpeed{ 1.f }, mAngularSpeed{ 1.f }, mJumpHeight{ 10.f }, mJump{ false }
	{
		if (!mActor->rootTransform())
			mActor->rootTransform({ { 0.0f, 1.5f, 0.0f }, {} });

		initialize();
	}

	void Player::initialize()
	{
		mDeltaPos = {};
		mEuler = {};
	}

	void Player::awake()
	{
		CScript::awake();

		mMesh = mActor->component<scene::CSkeletalMesh>();
	}

	void Player::update(float deltaTime)
	{
		const ww::Input* kInput{ Service::get<ww::Input>() };
		bool isWalkingForward{ false };
		bool isWalkingBackward{ false };

		scene::CTransform* actorTr{ mActor->rootTransform() };
		math::Transf tr{ actorTr->transform() };

		float speed{ mIsInAir ? mSpeed * 0.8f : mSpeed };
		if (kInput->key(ww::EKey::W).isDown())
		{
			mDeltaPos += tr.forward() * deltaTime * speed;
			isWalkingForward = true;
		}
		else if (kInput->key(ww::EKey::S).isDown())
		{
			mDeltaPos += tr.forward() * -deltaTime * speed;
			isWalkingBackward = true;
		}

		if (kInput->key(ww::EKey::A).isDown())
			mDeltaPos += tr.right() * -deltaTime * speed;
		else if (kInput->key(ww::EKey::D).isDown())
			mDeltaPos += tr.right() * deltaTime * speed;

		if (kInput->key(ww::EKey::SPACE).isPressed() && !mIsInAir)
			mJump = true;

		if (mEuler == math::Vec3f{})
			mEuler = actorTr->transform().rotation().toEuler();

		float angularSpeed{ mIsInAir ? mAngularSpeed * 0.8f : mAngularSpeed };
		const math::Vec2f& deltaAngle{ kInput->mouse().relativePosition() * deltaTime * angularSpeed };
		mEuler.mY -= deltaAngle.mX;

		if (mMesh && mIsWalking != (isWalkingForward || isWalkingBackward))
		{
			mIsWalking = (isWalkingForward || isWalkingBackward);

			if (isWalkingForward)
				mMesh->animation(0u);
			else if (isWalkingBackward)
			{
				const resources::SkeletalModel* kModel{ mMesh->model() };
				std::unique_ptr<anim::IAnimationInstance> anim{ std::make_unique<anim::AnimationInstance>(kModel->animation(0u)) };
				anim->playSpeed(-1.f);

				mMesh->animation(std::move(anim));
			}
			else
				mMesh->animation(nullptr);
		}
	}

	void Player::fixedUpdate()
	{
		mIsInAir = mActor->component<physic::ICCollider>()->linearVelocity().mY > 0.1f || mActor->component<physic::ICCollider>()->linearVelocity().mY < -0.1f;

		scene::CTransform* actorTr{ mActor->rootTransform() };
		math::Transf tr{ actorTr->transform() };
		tr.translate(mDeltaPos);
		tr.rotation(math::Quatf::fromEuler(mEuler));
		actorTr->transform(tr);

		mDeltaPos = {};

		if (mJump)
		{
			mJump = false;
			mActor->component<physic::ICCollider>()->addForce(math::Vec3f::skUp * 0.02f * mJumpHeight * 100.f, physic::EForceMode::IMPULSE);
		}
	}
}