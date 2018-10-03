#include "scene/IComponent.h"

#include "scene/Actor.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(IComponent)

	IComponent::IComponent()
		: mActor { nullptr }, mIsEnabled { true }
	{}

	IComponent::IComponent(Actor* actor)
		: mActor { actor }, mIsEnabled { true }
	{
		if (!mActor)
			THROW("failed to create component: no actor")

		initialize();
	}

	IComponent::~IComponent()
	{}

	void IComponent::initialize()
	{}

	const Actor* IComponent::actor()const
	{
		return mActor;
	}
	
	Actor* IComponent::actor()
	{
		return mActor;
	}

	void IComponent::moveTo(Actor* actor)
	{
		// Preventing detachment
		if (!actor)
		{
			LOG(LogWarning, "failed to set actor: initialized components must have an actor")
			return;
		}

		// Notifying actor of the move
		if (mActor == actor)
			return;

		if (mActor)
			Actor::moveTo(this, actor);

		// Updating the component's information
		Actor* previousActor = mActor;
		mActor = actor;

		// Attachment callback
		onAttach(previousActor);

		if (previousActor)
			mOnMoved(static_cast<Actor*&&>(previousActor));
	}
	
	void IComponent::awake()
	{}

	void IComponent::destroy() const
	{
		mOnDisabled(true);
	}

	void IComponent::enable(bool enabled)
	{
		if (mIsEnabled == enabled)
			return;

		mIsEnabled = enabled;

		if (mIsEnabled)
			mOnEnabled();
		else
			mOnDisabled(false);
	}
	
	bool IComponent::isEnabled() const
	{
		return mIsEnabled;
	}
}