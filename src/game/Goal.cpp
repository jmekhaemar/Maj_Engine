#include "Goal.h"

#include <scene/SceneHandler.h>
#include <gui/CWidget.h>

namespace maj::script
{
	DEFINE_HASH_OVERLOAD(Goal)

	Goal::Goal(scene::Actor* actor)
		: scene::CScript{ actor }, mTriggerEvent{ 0u }
	{
		initialize();
	}

	void Goal::initialize()
	{
		// Enable/Disable
		mOnEnabled += [this]() -> void
		{
			mTriggerEvent = mActor->mOnTriggerEnter + std::bind(&Goal::onTriggerEnter, this, std::placeholders::_1);
		};

		mOnDisabled += [this](bool destroyed) -> void
		{
			if (mActor)
				mActor -= mTriggerEvent;
		};
	}

	void Goal::update(float deltaTime)
	{
		scene::CTransform* actorTr{ mActor->rootTransform() };
		math::Transf tr{ actorTr->transform() };
		tr.rotate({ 45.f * deltaTime, { 0.f, 1.f, 0.f } });
		actorTr->transform(tr);

		mTimer.step();
	}

	void Goal::onAttach(scene::Actor* previousActor)
	{
		CScript::onAttach(previousActor);

		if (previousActor)
			previousActor -= mTriggerEvent;

		if (mIsEnabled)
		{
			mTriggerEvent = mActor->mOnTriggerEnter + std::bind(&Goal::onTriggerEnter, this, std::placeholders::_1);
		}
	}

	void Goal::onTriggerEnter(scene::Actor* actor)
	{
		LOG(LogInfo, "Goal trigger enter with" << actor->name())

		mTimer.set(1.f, [&]() -> void
		{
			Service::get<scene::SceneHandler>()->reset();
		});
		Service::get<scene::SceneHandler>()->activeScene()->actor("victory")->component<gui::CWidget>()->enable(true);
		mTimer.start();
	}
}