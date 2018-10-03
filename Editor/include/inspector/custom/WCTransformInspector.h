#ifndef _WCTRANSFORM_INSPECTOR_H_
#define _WCTRANSFORM_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <scene/CTransform.h>

namespace maj::editor
{
	class WCTransformInspector : public WInspectorComponent
	{
	public:
		WCTransformInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onValueChanged();
		void onStaticChanged();

		scene::CTransform* mTransform;
	};
}

#include "WCTransformInspector.inl"

#endif