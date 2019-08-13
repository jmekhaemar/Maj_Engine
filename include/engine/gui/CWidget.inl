#ifndef _CWIDGET_INL_
#define _CWIDGET_INL_

#include "gui/CWidget.h"

namespace maj::gui
{
	template <typename T>
	T* CWidget::element(uint32_t index)
	{
		static_assert(std::is_base_of<IElement, T>::value, "failed to get widget element: type doesn't inherit from IElement");

		uint32_t elementCount { static_cast<uint32_t>(mElements.size()) };
		if (index >= elementCount)
		{
			LOG(LogWarning, "failed to get gui element: index out of range")
			return nullptr;
		}

		return dynamic_cast<T*>(mElements[index]);
	}

	template <typename T>
	const T* CWidget::element(uint32_t index) const
	{
		static_assert(std::is_base_of<IElement, T>::value, "failed to get widget element: type doesn't inherit from IElement");

		uint32_t elementCount { static_cast<uint32_t>(mElements.size()) };
		if (index >= elementCount)
		{
			LOG(LogWarning, "failed to get gui element: index out of range")
			return nullptr;
		}

		return dynamic_cast<T*>(mElements[index]);
	}

	template <typename T, typename... Args>
	T* CWidget::addElement(Args&&... args)
	{
		static_assert(std::is_base_of<IElement, T>::value, "failed to add gui element: type doesn't inherit from IElement");

		T* element { new T { this, std::forward<Args>(args)... } };
		mElements.emplace_back(element);

		if (mSpace == ECoordSpace::WORLD)
		{
			scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
			element->camera(activeScene->cameras().main()->buffer());
		}

		return element;
	}
}

#endif