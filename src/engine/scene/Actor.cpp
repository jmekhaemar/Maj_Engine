#include "scene/Actor.h"

#include "Engine.h"
#include "scene/CTransform.h"
#include "scene/CCamera.h"
#include "physic/CollisionData.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(Actor)

	void Actor::moveTo(IComponent* component, Actor* actor)
	{
		// Checking parameters
		if (!actor)
		{
			LOG(LogWarning, "failed to attach component: initialized components must have an actor")
			return;
		}

		if (!component)
		{
			LOG(LogWarning, "failed to attach component to actor \'" << actor->mName << "\': only initialized components can be attached")
			return;
		}

		CTransform* transform { dynamic_cast<CTransform*>(component) };
		if (transform)
		{
			if (transform->actor())
				LOG(LogWarning, "failed to attach component to actor \'" << actor->mName << "\': transform cannot be moved")

			return;
		}

		// Removing the component from its former actor
		Actor* oldActor { component->actor() };
		if (oldActor == actor)
			return;

		if (oldActor)
		{
			bool found { false };
			for (std::vector<IComponent*>::iterator it { oldActor->mComponents.begin() } ; it != oldActor->mComponents.end() && !found ;)
			{
				if (component != *it)
				{
					++it;
					continue;
				}

				it = oldActor->mComponents.erase(it);
				found = true;
			}

			if (!found)
			{
				LOG(LogWarning, "failed to attach component to actor \'" << actor->mName << "\': component's former actor mismatch")
				return;
			}
		}

		// Adding the component to its new actor
		actor->mComponents.emplace_back(component);

		// Notifying the component of the move
		component->moveTo(actor);
	}

	Actor::Actor(JsonNode* root)
		: mName { "default" }, mComponents {}, mRootTransform { nullptr }, mParent { nullptr }, mChildren {}
	{
#ifdef MAJ_EDITOR
		mIsSelected = false;
#endif

		load(root);
		initialize();
	}

	Actor::Actor()
		: mName { "Root" }, mComponents {}, mRootTransform{ nullptr }, mParent{ nullptr }, mChildren{}, mDontDestroyOnLoad{ false }
	{
#ifdef MAJ_EDITOR
		mIsSelected = false;
#endif
	}
	
	Actor::Actor(const std::string& kName)
		: mName { kName }, mComponents {}, mRootTransform { nullptr }, mParent { nullptr }, mChildren {}, mDontDestroyOnLoad{ false }
	{
#ifdef MAJ_EDITOR
		mIsSelected = false;
#endif
	}

	Actor::~Actor()
	{
#ifdef MAJ_EDITOR
		if (mIsSelected)
			Service::get<gizmo::GizmoHandler>()->selectActor(nullptr);
#endif

		removeComponents();
		removeChildren();
	}

	void Actor::initialize()
	{
		if (mRootTransform)
			mRootTransform->moveTo(this);

		for (IComponent* component : mComponents)
			component->moveTo(this);

		for (Actor* child : mChildren)
			child->mParent = this;
	}

	const std::string& Actor::name() const
	{
		return mName;
	}

	void Actor::name(const std::string& kStr)
	{
		mName = kStr;
	}

	const std::vector<IComponent*>& Actor::components() const
	{
		return mComponents;
	}

	uint8_t Actor::componentCount() const
	{
		return static_cast<uint8_t>(mComponents.size());
	}

	void Actor::removeComponents()
	{
		if (mRootTransform)
		{
			mRootTransform->destroy();
			delete mRootTransform;
		}

		mRootTransform = nullptr;

		const std::vector<IComponent*> kComponents { mComponents };
		mComponents.clear();

		for (std::vector<IComponent*>::const_iterator it { kComponents.begin() } ; it != kComponents.end() ; ++it)
		{
			if (!(*it))
				continue;

			(*it)->destroy();
			delete *it;
		}
	}

	void Actor::removeComponent(uint8_t index)
	{
		uint8_t componentCount {static_cast<uint8_t>(mComponents.size()) };
		if (index >= componentCount)
		{
			LOG(LogWarning, "failed to remove component: index out of range")
			return;
		}

		IComponent* component { mComponents[index] };
		mComponents.erase(mComponents.begin() + index);

		if (component)
		{
			component->destroy();
			delete component;
		}
	}

	void Actor::removeComponent(IComponent* component)
	{
		if (!component)
			return;

		for (std::vector<IComponent*>::iterator it { mComponents.begin() } ; it != mComponents.end() ; ++it)
		{
			if (component != *it)
				continue;

			mComponents.erase(it);

			component->destroy();
			delete component;
			return;
		}

		LOG(LogWarning, "failed to remove component: component in actor \'" << mName << "\' not found")
	}

	void Actor::rootTransform(const math::Transf& trans)
	{
		if (!mRootTransform)
		{
			mRootTransform = new CTransform { this };
			mRootTransform->awake();
		}

		mRootTransform->transform(trans);
	}

	const CTransform* Actor::rootTransform() const
	{
		return mRootTransform;
	}

	CTransform* Actor::rootTransform()
	{
		return mRootTransform;
	}

	bool Actor::isRoot() const
	{
		return !mParent;
	}

	const Actor* Actor::parent() const 
	{
		return mParent;
	}

	Actor* Actor::parent()
	{
		return mParent;
	}

	void Actor::parent(Actor* parent)
	{
		if (!parent)
		{
			LOG(LogWarning, "failed to set parent: actor \'" << mName << "\' must have a parent")
			return;
		}

		if (mParent)
		{
			// Iterate over siblings until find itself to remove itself from its parent
			bool found { false };
			for (std::vector<Actor*>::iterator it { mParent->mChildren.begin() } ; it != mParent->mChildren.end() && !found ;)
			{
				if (this != *it)
				{
					++it;
					continue;
				}

				it = mParent->mChildren.erase(it);
				found = true;
			}
		}

		mParent = parent;
		mParent->mChildren.emplace_back(this);
	}

	void Actor::parent(std::nullptr_t nullPointer)
	{
		(void) nullPointer;

		// Finding the root node
		Actor* root { mParent };
		while (root && root->mParent)
			root = root->mParent;

		// If already on the root, do nothing
		if (root == mParent)
			return;

		// Iterate over siblings until find itself to remove itself from its parent
		bool found { false };
		for (std::vector<Actor*>::iterator it { mParent->mChildren.begin() } ; it != mParent->mChildren.end() && !found ;)
		{
			if (this != *it)
			{
				++it;
				continue;
			}

			it = mParent->mChildren.erase(it);
			found = true;
		}

		mParent = root;
		mParent->mChildren.emplace_back(this);
	}

	const std::vector<Actor*>& Actor::children()const
	{
		return mChildren;
	}

	uint8_t Actor::childCount() const
	{
		return static_cast<uint8_t>(mChildren.size());
	}

	void Actor::addChild(Actor* child)
	{
		if (!child)
			return;

		if (child->mParent)
		{
			// Iterate over its siblings to remove it from its parent
			bool found { false };
			for (std::vector<Actor*>::iterator it { child->mParent->mChildren.begin() }; it != child->mParent->mChildren.end() && !found ;)
			{
				if (child != *it)
				{
					++it;
					continue;
				}

				it = child->mParent->mChildren.erase(it);
				found = true;
			}
		}

		child->mParent = this;
		mChildren.emplace_back(child);
	}

	void Actor::removeChildren()
	{
		const std::vector<Actor*> kActors { mChildren };
		mChildren.clear();

		for (std::vector<Actor*>::const_iterator it { kActors.begin() }; it != kActors.end() ; ++it)
			delete *it;
	}

	void Actor::removeChild(uint8_t index)
	{
		uint8_t childCount { static_cast<uint8_t>(mChildren.size()) };
		if (index >= childCount)
		{
			LOG(LogWarning, "failed to remove child: index out of range")
			return;
		}

		Actor* child { mChildren[index] };
		mChildren.erase(mChildren.begin() + index);

		delete child;
	}

	void Actor::removeChild(const std::string& kStr)
	{
		for (std::vector<Actor*>::iterator it { mChildren.begin() }; it != mChildren.end() ; ++it)
		{
			Actor* child { *it };
			if (kStr != child->name())
				continue;

			mChildren.erase(it);
			delete child;
			return;
		}

		LOG(LogWarning, "failed to remove child: child actor \'"<< kStr <<"\' not found in actor \'" << mName << "\'")
	}

	void Actor::removeChild(const Actor* kChild)
	{
		if (!kChild)
			return;

		for (std::vector<Actor*>::iterator it { mChildren.begin() }; it != mChildren.end() ; ++it)
		{
			if (kChild != *it)
				continue;

			mChildren.erase(it);
			delete kChild;
			return;
		}

		LOG(LogWarning, "failed to remove child: child actor \'" << kChild->name() << "\' not found in actor \'" << mName << "\'")
	}
	
	const Actor* Actor::child(uint8_t index) const
	{
		uint8_t childCount { static_cast<uint8_t>(mChildren.size()) };
		if (index >= childCount)
		{
			LOG(LogWarning, "failed to get child actor: index out of range")
			return nullptr;
		}

		return mChildren[index];
	}

	Actor* Actor::child(uint8_t index)
	{
		uint8_t childCount { static_cast<uint8_t>(mChildren.size()) };
		if (index >= childCount)
		{
			LOG(LogWarning, "failed to get child actor: index out of range")
			return nullptr;
		}

		return mChildren[index];
	}
	
	const Actor* Actor::child(const std::string& kStr)const
	{
		uint8_t childCount { static_cast<uint8_t>(mChildren.size()) };
		for (uint8_t index { 0u }; index < childCount ; ++index)
		{
			if (kStr != mChildren[index]->name())
				continue;

			return mChildren[index];
		}

		LOG(LogWarning, "failed to get child actor: child actor \'" << kStr << "\' not found in actor \'" << mName << "\'")
		return nullptr;
	}

	Actor* Actor::child(const std::string& kStr)
	{
		uint8_t childCount { static_cast<uint8_t>(mChildren.size()) };
		for (uint8_t index { 0u }; index < childCount ; ++index)
		{
			if (kStr != mChildren[index]->name())
				continue;

			return mChildren[index];
		}

		LOG(LogWarning, "failed to get child actor: child actor \'" << kStr << "\' not found in actor \'" << mName << "\'")
		return nullptr;
	}

	void Actor::awake()
	{
		if (mRootTransform)
			mRootTransform->awake();

		for (IComponent* component : mComponents)
			component->awake();

		for (Actor* child : mChildren)
			child->awake();
	}

#ifdef MAJ_EDITOR
	void Actor::select(bool selected)
	{
		if (mIsSelected == selected)
			return;

		mIsSelected = selected;
		mOnSelected(static_cast<bool&&>(mIsSelected));
	}

	bool Actor::isSelected() const
	{
		return mIsSelected;
	}
#endif
}