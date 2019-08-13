#ifndef _SCENE_INL_
#define _SCENE_INL_

#include "scene/Scene.h"

namespace maj::scene
{
	template <typename T, typename... Args>
	T* Scene::addActor(const std::string& kStr, Args&&... args)
	{
		static_assert(std::is_base_of<Actor, T>::value, "failed to add actor: type doesn't inherit from Actor");

		Actor* temp { actor(mRoot, kStr) };
		if (temp)
		{
			LOG(LogWarning, "failed to add actor: actor \'" << kStr << "\' already in scene \'" << mName << "\'")
			return nullptr;
		}

		return mRoot->addChild<T>(kStr, std::forward<Args>(args)...);
	}
	
	template<typename T>
	const T* Scene::firstComponent(const Actor* kNode) const
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get first component: type doesn't inherit from IComponent");

		if (!kNode)
			return nullptr;
		
		const T* temp { kNode->component<T>() };
		if (temp)
			return temp;

		uint8_t childrenSize { kNode->childCount() };
		for (uint8_t index { 0u } ; index < childrenSize ; ++index)
		{
			temp = firstComponent<T>(kNode->child(index));
			if (temp)
				return temp;
		}

		return nullptr;
	}

	template<typename T>
	const T* Scene::firstComponent() const
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get first component: type doesn't inherit from IComponent");

		const T* temp { firstComponent<T>(mRoot) };
		if (!temp)
		{
			LOG(LogWarning, "failed to get first component : no component of this type in scene \'" << mName << "\'")
		}
		
		return temp;
	}

	template<typename T>
	T* Scene::firstComponent(Actor* node)
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get first component: type doesn't inherit from IComponent");

		if (!node)
			return nullptr;
		
		T* temp { node->component<T>() };
		if (temp)
			return temp;

		uint8_t childrenSize { node->childCount() };
		for (uint8_t index { 0u } ; index < childrenSize ; ++index)
		{
			temp = firstComponent<T>(node->child(index));
			if (temp)
				return temp;
		}

		return nullptr;
	}

	template<typename T>
	T* Scene::firstComponent()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "failed to get first component: type doesn't inherit from IComponent");

		T* temp { firstComponent<T>(mRoot) };
		if (!temp)
		{
			LOG(LogWarning, "failed to get first component : no component of this type in scene \'" << mName << "\'")
		}
		
		return temp;
	}
}

#endif