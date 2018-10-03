#ifndef _IGIZMO_H_
#define _IGIZMO_H_

#include "scene/IComponent.h"
#include "gizmo/GizmoHandler.h"

namespace maj::gizmo
{
	class IGizmo : public NonCopyable
	{
	public:
		IGizmo(scene::IComponent* component);
		virtual ~IGizmo() = default;
		
		virtual void enable(GizmoHandler& handler, bool enabled) = 0;

	protected:
		virtual void draw() const = 0;

		scene::IComponent* mComponent;

	private:
		uint32_t mDrawIndex;
	};
}

#endif