#ifndef _ICOMPONENT_H_
#define _ICOMPONENT_H_

#include <ISerializable.h>
#include <Event.h>

namespace maj::scene
{
	class Actor;

	class IComponent : public NonCopyable, public ISerializable
	{
		ENABLE_META(IComponent)

	public:
		IComponent(Actor* actor);
		virtual ~IComponent();

		INITIALIZE_SIGNATURE();

		const Actor* actor() const;
		Actor* actor();
		virtual void moveTo(Actor* actor);

		virtual void awake();

		void destroy() const;
		void enable(bool enabled);
		bool isEnabled() const;
		
		Event<void> mOnEnabled;
		Event<void, bool> mOnDisabled;
		Event<void, Actor*> mOnMoved;

	protected:
		IComponent();

		virtual void onAttach(Actor* previousActor) = 0;

		Actor* mActor;
		bool mIsEnabled;
	};
}

META_CLASS(maj::scene::IComponent, 1u)
META_FIELD(maj::scene::IComponent, bool, mIsEnabled, 0u)

#endif