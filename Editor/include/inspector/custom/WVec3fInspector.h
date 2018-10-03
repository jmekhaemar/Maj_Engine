#ifndef _WVEC3F_INSPECTOR_H_
#define _WVEC3F_INSPECTOR_H_

#include "inspector/WInspectorComponent.h"

namespace maj::editor
{
	class WVec3fInspector : public WInspectorComponent
	{
	public:
		WVec3fInspector(QWidget* parent, QFormLayout* layout = nullptr, std::function<void()> callback = nullptr);

		template<typename T>
		void load(T* component);
	};
}

#include "inspector/custom/WVec3fInspector.inl"

#endif
