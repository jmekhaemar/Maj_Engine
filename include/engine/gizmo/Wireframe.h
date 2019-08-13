#ifndef _WIREFRAME_H_
#define _WIREFRAME_H_

#include <MaterialInstance.h>
#include <Transform.h>

#include "resources/Model.h"
#include "gizmo/IGizmo.h"

namespace maj::gizmo
{
	class Wireframe final : public IGizmo
	{
	public:
		static const char* skMaterialName;
		static const char* skLayoutName;

		Wireframe(scene::IComponent* component, const resources::Model* kModel);
		virtual ~Wireframe();

		void transform(const std::function<math::Transf()>& kTransform);

		const math::Vec3Holder<float>& color() const;
		void color(const math::Vec3Holder<float>& kColor);

		virtual void enable(GizmoHandler& handler, bool enabled) override;
		
	private:
		virtual void draw() const override;

		void updateMaterial();
		void createMaterial();

		rhi::MaterialInstance* mMaterialInstance;
		const resources::Model* mkModel;
		rhi::Buffer::SubBuffer* mBuffer;

		uint32_t mCameraEventIndex;
		std::function<math::Transf()> mTransform;
		math::Vec3Holder<float> mColor;
	};
}

#endif