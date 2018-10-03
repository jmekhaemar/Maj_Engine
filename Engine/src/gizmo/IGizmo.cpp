#include "gizmo/IGizmo.h"

#include "scene/SceneHandler.h"

namespace maj::gizmo
{
	IGizmo::IGizmo(scene::IComponent* component)
		: mComponent { component }, mDrawIndex { 0u }
	{
		if (!mComponent)
			THROW("failed to create gizmo: no component")
	}

	void IGizmo::enable(GizmoHandler& handler, bool enabled)
	{
		if (enabled)
			mDrawIndex = handler.mOnDraw + std::bind(&IGizmo::draw, this);
		else
			handler.mOnDraw -= mDrawIndex;
	}
}