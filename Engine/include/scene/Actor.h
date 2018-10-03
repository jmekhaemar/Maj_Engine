#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <vector>
#include <string>

#include <Event.h>

#include "scene/CTransform.h"

namespace maj::physic
{
	struct CollisionData;
}

namespace maj::scene
{
	class Actor : public ISerializable
	{
		ENABLE_META(Actor)

	public:
		static void moveTo(IComponent* component, Actor* newActor);

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Actor)

		Actor();
		Actor(const std::string& kName);
		virtual ~Actor();

		const std::string& name() const;
		void name(const std::string& kStr);

		const std::vector<IComponent*>& components() const;
		uint8_t componentCount() const;
		template <typename T>
		uint8_t componentCount() const;

		template<typename T>
		const T* component(uint8_t index) const;
		template<typename T>
		T* component(uint8_t index);

		template<typename T>
		const T* component() const;
		template<typename T>
		T* component();

		template<typename T, typename... Args>
		T* addComponent(Args&&... args);

		void removeComponents();
		void removeComponent(uint8_t index);
		void removeComponent(IComponent* component);

		void rootTransform(const math::Transf& trans);
		const CTransform* rootTransform()const;
		CTransform* rootTransform();

		bool isRoot() const;

		const Actor* parent() const;
		Actor* parent();
		void parent(Actor* actor);
		void parent(std::nullptr_t nullPointer);

		const std::vector<Actor*>& children() const;
		uint8_t childCount() const;

		template <typename T, typename... Args>
		T* addChild(Args&&... args);
		void addChild(Actor* child);
		
		void removeChildren();
		void removeChild(uint8_t index);
		void removeChild(const std::string& kStr);
		void removeChild(const Actor* kChild);

		const Actor* child(uint8_t index) const;
		Actor* child(uint8_t index);

		const Actor* child(const std::string& kStr) const;
		Actor* child(const std::string& kStr);

		virtual void awake();

#ifdef MAJ_EDITOR
		void select(bool selected);

		bool isSelected() const;
#endif

		Event<void> mOnDraw;
		Event<void, float> mOnUpdate;
		Event<void, float> mOnLateUpdate;
		Event<void> mOnFixedUpdate;

		Event<void, Actor*> mOnTriggerEnter;
		Event<void, Actor*> mOnTriggerExit;

		Event<void, physic::CollisionData*> mOnCollisionEnter;
		Event<void, physic::CollisionData*> mOnCollisionStay;
		Event<void, physic::CollisionData*> mOnCollisionExit;

#ifdef MAJ_EDITOR
		Event<void, bool> mOnSelected;
#endif

	protected:
		std::string mName;

		std::vector<IComponent*> mComponents;
		CTransform* mRootTransform;
		
		Actor* mParent;
		std::vector<Actor*> mChildren;

		bool mDontDestroyOnLoad; // TODO remove ? never used

#ifdef MAJ_EDITOR
		bool mIsSelected;
#endif
	};
}

#include "scene/Actor.inl"

META_CLASS(maj::scene::Actor, 4u)
META_FIELD(maj::scene::Actor, maj::scene::CTransform*, mRootTransform, 0u)
META_FIELD_ARRAY(maj::scene::Actor, std::vector<maj::scene::IComponent*>, mComponents, 1u)
META_FIELD_ARRAY(maj::scene::Actor, std::vector<maj::scene::Actor*>, mChildren, 2u)
META_FIELD(maj::scene::Actor, std::string, mName, 3u)

#endif