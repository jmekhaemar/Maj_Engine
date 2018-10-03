#include "CameraBehavior.h"

#include <Vec2.h>
#include <Input.h>
#include <physic/RayCast.h>
#include <Window.h>

namespace maj::script
{
	DEFINE_HASH_OVERLOAD(CameraBehavior)

	CameraBehavior::CameraBehavior(scene::Actor* actor)
		: scene::CScript{ actor }, mHoldObject{ nullptr }, mSpeed{ 1.f }, mMaxAngleX{ 20.f }
	{
		initialize();
	}

	void CameraBehavior::initialize()
	{
		mHoldObject = nullptr;
		mEuler = {};
	}

	void CameraBehavior::update(float deltaTime)
	{
		scene::CTransform* actorTr{ mActor->rootTransform() };
		const ww::Input* kInput{ Service::get<ww::Input>() };

		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		viewport.cursorMode(ww::Cursor::EMode::FORCE);

		if (mEuler == math::Vec3f{})
			mEuler = actorTr->transform().rotation().toEuler();

		const math::Vec2f& deltaAngle{ kInput->mouse().relativePosition() * deltaTime * mSpeed };

		if (mEuler.mX <= mMaxAngleX && mEuler.mX >= -mMaxAngleX)
			mEuler.mX -= deltaAngle.mY;
		else if (mEuler.mX > mMaxAngleX)
			mEuler.mX = mMaxAngleX - 0.05f;
		else
			mEuler.mX = -mMaxAngleX + 0.05f;

		if (kInput->mouse().key(ww::EMouseKey::LEFT).isPressed())
		{
			physic::RayCast ray{ actorTr->global().position(), actorTr->global().forward(), 1.5f, physic::ELayer::LAYER_3 };
			physic::RayCastHit hit{};
			if (ray(hit))
			{
				LOG(LogInfo, "raycast hit " << hit.mCollider->actor()->name())
				mHoldObject = hit.mCollider;
			}
		}
		else if (mHoldObject && kInput->mouse().key(ww::EMouseKey::LEFT).isReleased())
		{
			mHoldObject->useGravity(true);
			mHoldObject = nullptr;
		}

		if (mHoldObject && kInput->mouse().key(ww::EMouseKey::LEFT).isDown())
		{
			mHoldObject->actor()->rootTransform()->position(actorTr->global().position() + actorTr->global().forward() * 1.f);
			mHoldObject->linearVelocity({ 0.f });
			mHoldObject->angularVelocity({ 0.f });
			mHoldObject->useGravity(false);
		}
	}

	void CameraBehavior::fixedUpdate()
	{
		scene::CTransform* actorTr{ mActor->rootTransform() };

		math::Transf transform{ actorTr->transform() };
		transform.rotation(math::Quatf::fromEuler(mEuler));
		actorTr->transform(transform);
	}
}