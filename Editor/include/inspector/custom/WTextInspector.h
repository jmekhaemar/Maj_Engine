#ifndef _WTEXT_INSPECTOR_H_
#define _WTEXT_INSPECTOR_H_

#include "inspector/WInspectorComponent.h"

#include <gui/Text.h>

namespace maj::editor
{
	class WTextInspector : public WInspectorComponent
	{
	public:
		WTextInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		gui::Text* mText;
	};
}

#include "inspector/custom/WTextInspector.inl"

#endif