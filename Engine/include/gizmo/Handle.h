#ifndef _GIZMO_HANDLE_H_
#define _GIZMO_HANDLE_H_

#include <MaterialInstance.h>

#include "gizmo/IGizmo.h"
#include "scene/CTransform.h"

namespace maj::gizmo
{
	class Handle : public IGizmo
	{
	public:
		static const char* skMaterialName;
		static const char* skLayoutName;

		Handle(scene::IComponent* transform);
		virtual ~Handle();

		virtual void enable(GizmoHandler& handler, bool enabled) override;

	private:
		virtual void draw() const override;

		void updateMaterial();
		void createMaterial();

		rhi::MaterialInstance* mMaterialInstance;
		rhi::Buffer::SubBuffer* mBuffer;

		uint32_t mCameraEventIndex;
	};
}

#endif