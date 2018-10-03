#include "DeathZone.h"

namespace maj::script
{
	DEFINE_HASH_OVERLOAD(DeathZone)

	DeathZone::DeathZone(scene::Actor* actor)
		: scene::CScript { actor }, mTriggerEvent { 0u }
	{
		initialize();
	}

	void DeathZone::initialize()
	{
		// Enable/Disable
		mOnEnabled += [this] () -> void
		{
			mTriggerEvent = mActor->mOnTriggerEnter + std::bind(&DeathZone::onTriggerEnter, this, std::placeholders::_1);
		};

		mOnDisabled += [this] (bool destroyed) -> void
		{
			(void) destroyed;

			if (mActor)
				mActor -= mTriggerEvent;
		};
	}

	void DeathZone::onAttach(scene::Actor* previousActor)
	{
		CScript::onAttach(previousActor);

		if (previousActor)
			previousActor -= mTriggerEvent;

		if (mIsEnabled)
		{
			mTriggerEvent = mActor->mOnTriggerEnter + std::bind(&DeathZone::onTriggerEnter, this, std::placeholders::_1);
		}
	}

	void DeathZone::onTriggerEnter(scene::Actor* actor)
	{
		LOG(LogInfo, "DeathZone trigger enter with" << actor->name())

		scene::CTransform* transform { actor->rootTransform() };
		if (transform)
			transform->position({ 0.f, 5.f, 0.f });
	}
}