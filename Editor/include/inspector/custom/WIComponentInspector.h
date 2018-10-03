#ifndef _WICOMPONENT_INSPECTOR_H_
#define _WICOMPONENT_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <scene/IComponent.h>

namespace maj::editor
{
	class WIComponentInspector : public WInspectorComponent
	{
	public:
		WIComponentInspector(QWidget* parent, QFormLayout* layout);

		template<typename T>
		void load(T* component);

	private:
		void onValueChanged();

		scene::IComponent* mComponent;
	};
}

#include "WIComponentInspector.inl"

#endif