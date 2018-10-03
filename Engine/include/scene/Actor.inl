#ifndef _ACTOR_INL_
#define _ACTOR_INL_

#include  <type_traits>

#include <Logging.h>

#include "scene/Actor.h"

namespace maj::scene
{
	template <typename T>
	uint8_t Actor::componentCount() const
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get component count: type doesn't inherit from IComponent");

		uint8_t count { 0u };
		for (std::vector<IComponent*>::const_iterator it { mComponents.begin() } ; it != mComponents.end() ; ++it)
		{
			if (dynamic_cast<const T*>(*it))
				++count;
		}

		return count;
	}

	template <typename T>
	T* Actor::component(uint8_t index)
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get component: type doesn't inherit from IComponent");

		uint8_t componentCount { static_cast<uint8_t>(mComponents.size()) };
		if (index >= componentCount)
		{
			LOG(LogWarning, "failed to get component: index out of range")
			return nullptr;
		}

		return dynamic_cast<T*>(mComponents[index]);
	}

	template <typename T>
	const T* Actor::component(uint8_t index) const
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get component: type doesn't inherit from IComponent");

		uint8_t componentCount { static_cast<uint8_t>(mComponents.size()) };
		if (index >= componentCount)
		{
			LOG(LogWarning, "failed to get component: index out of range")
			return nullptr;
		}

		return dynamic_cast<T*>(mComponents[index]);
	}

	template<typename T>
	T* Actor::component()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get component: type doesn't inherit from IComponent");

		for (std::vector<IComponent*>::iterator it { mComponents.begin() } ; it != mComponents.end() ; ++it)
		{
			T* temp { dynamic_cast<T*>(*it) };
			if (temp)
				return temp;
		}

		return nullptr;
	}

	template<typename T>
	const T* Actor::component() const
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get component: type doesn't inherit from IComponent");

		for (std::vector<IComponent*>::const_iterator it { mComponents.begin() } ; it != mComponents.end() ; ++it)
		{
			const T* temp { dynamic_cast<const T*>(*it) };
			if (temp)
				return temp;
		}

		return nullptr;
	}

	template<typename T, typename... Args>
	T* Actor::addComponent(Args&&... args)
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to add component: type doesn't inherit from IComponent");
		static_assert(!std::is_same<T, CTransform>::value, "failed to add component: can't add component of type CTransform, use rootTransform");

		T* component { new T { this, std::forward<Args>(args)... } };
		mComponents.emplace_back(component);

		component->awake();
		return component;
	}

	template <typename T, typename... Args>
	T* Actor::addChild(Args&&... args)
	{
		static_assert(std::is_base_of<Actor, T>::value, "failed to add child: type doesn't inherit from Actor");

		T* child { new T { std::forward<Args>(args)... } };

		child->mParent = this;
		mChildren.emplace_back(child);

		child->awake();
		return child;
	}
}

#endif