#ifndef _WCCAMERA_INSPECTOR_H_
#define _WCCAMERA_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <scene/CCamera.h>

namespace maj::editor
{
	class WCCameraInspector : public WInspectorComponent
	{
	public:
		WCCameraInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onValueChanged();

		scene::CCamera* mCamera;
	};
}

#include "WCCameraInspector.inl"

#endif