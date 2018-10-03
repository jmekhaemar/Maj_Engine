#ifndef _WVEC2UI_INSPECTOR_H_
#define _WVEC2UI_INSPECTOR_H_

#include "inspector/WInspectorComponent.h"

namespace maj::editor
{
	class WVec2uiInspector : public WInspectorComponent
	{
	public:
		WVec2uiInspector(QWidget* parent, QFormLayout* layout = nullptr, std::function<void()> callback = nullptr);

		template<typename T>
		void load(T* component);
	};
}

#include "inspector/custom/WVec2uiInspector.inl"

#endif
