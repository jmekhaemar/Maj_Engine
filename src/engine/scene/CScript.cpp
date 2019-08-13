#include "scene/CScript.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(CScript)

	CScript::CScript(Actor* actor)
		: IComponent { actor }, mUpdateIndex { 0u }, mLateUpdateIndex { 0u }, mFixedUpdateIndex { 0u }
	{
		initialize();
		onAttach(nullptr);
	}

	CScript::~CScript()
	{}

	void CScript::initialize()
	{
		mOnEnabled += [this] () -> void
		{
			mUpdateIndex = mActor->mOnUpdate + std::bind(&CScript::update, this, std::placeholders::_1);
			mLateUpdateIndex = mActor->mOnLateUpdate + std::bind(&CScript::lateUpdate, this, std::placeholders::_1);
			mFixedUpdateIndex = mActor->mOnFixedUpdate + std::bind(&CScript::fixedUpdate, this);
		};

		mOnDisabled += [this] (bool destroyed) -> void
		{
			(void)destroyed;

			if (mActor)
			{
				mActor->mOnUpdate -= mUpdateIndex;
				mActor->mOnLateUpdate -= mLateUpdateIndex;
				mActor->mOnFixedUpdate -= mFixedUpdateIndex;
			}
		};
	}

	void CScript::update(float deltaTime)
	{
		(void) deltaTime;
	}

	void CScript::lateUpdate(float deltaTime)
	{
		(void)deltaTime;
	}

	void CScript::fixedUpdate()
	{}

	void CScript::onAttach(Actor* previousActor)
	{
		if (previousActor)
		{
			previousActor->mOnUpdate -= mUpdateIndex;
			previousActor->mOnLateUpdate -= mLateUpdateIndex;
			previousActor->mOnFixedUpdate -= mFixedUpdateIndex;
		}

		if (mIsEnabled)
		{
			mUpdateIndex = mActor->mOnUpdate + std::bind(&CScript::update, this, std::placeholders::_1);
			mLateUpdateIndex = mActor->mOnLateUpdate + std::bind(&CScript::lateUpdate, this, std::placeholders::_1);
			mFixedUpdateIndex = mActor->mOnFixedUpdate + std::bind(&CScript::fixedUpdate, this);
		}
	}
}