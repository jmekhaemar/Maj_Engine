#ifndef _GRID_H_
#define _GRID_H_

#include <MaterialInstance.h>

#include "gizmo/IGizmo.h"

namespace maj::gizmo
{
	class Grid final : public IGizmo
	{
	public:
		static const char* skMaterialName;
		static const char* skLayoutName;

		Grid(scene::IComponent* component);
		virtual ~Grid();

		virtual void enable(GizmoHandler& handler, bool enabled) override;

	private:
		virtual void draw() const override;

		void updateMaterial();
		void createMaterial();

		rhi::MaterialInstance* mMaterialInstance;

		uint32_t mCameraEventIndex;
	};
}

#endif