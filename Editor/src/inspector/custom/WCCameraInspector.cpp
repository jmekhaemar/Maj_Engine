#include "inspector/custom/WCCameraInspector.h"

namespace maj::editor
{
	WCCameraInspector::WCCameraInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent{ parent, layout }
	{}

	void WCCameraInspector::onValueChanged()
	{
		mCamera->projection(meta::Field<scene::CCamera, 0u>::get(mCamera), 
							meta::Field<scene::CCamera, 1u>::get(mCamera), 
							meta::Field<scene::CCamera, 2u>::get(mCamera), 
							meta::Field<scene::CCamera, 3u>::get(mCamera));
	}
}