#ifndef _WSHADER_INSPECTOR_H_
#define _WSHADER_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <Shader.h>

namespace maj::editor
{
	class WShaderInspector : public WInspectorComponent
	{
	public:
		WShaderInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onValueChanged();

		rhi::Shader* mShader;
	};
}

#include "WShaderInspector.inl"

#endif
