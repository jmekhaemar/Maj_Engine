#include "inspector/IInspectorComponent.h"

namespace maj::editor
{
	IInspectorComponent::IInspectorComponent(std::function<void()> callback)
		: mCallback{ callback }
	{
	}

	void IInspectorComponent::update() {}
}