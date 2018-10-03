#ifndef _WVEC4F_INSPECTOR_H_
#define _WVEC4F_INSPECTOR_H_

#include "../WInspectorComponent.h"

namespace maj::editor
{
	class WVec4fInspector : public WInspectorComponent
	{
	public:
		WVec4fInspector(QWidget* parent, QFormLayout* layout = nullptr, std::function<void()> callback = nullptr);

		template<typename T>
		void load(T* component);
	};
}

#include "WVec4fInspector.inl"

#endif
