#ifndef _WIELEMENT_INSPECTOR_H_
#define _WIELEMENT_INSPECTOR_H_

#include "inspector/WInspectorComponent.h"

#include <gui/IElement.h>

namespace maj::editor
{
	class WIElementInspector : public WInspectorComponent
	{
	public:
		WIElementInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onDimensionChanged();
		void onValueChanged();

		gui::IElement* mElement;

		math::Vec2ui mPosition;
		math::Vec2ui mSize;
	};
}

#include "WIElementInspector.inl"

#endif