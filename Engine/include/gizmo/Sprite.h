#ifndef _GIZMO_SPRITE_H_
#define _GIZMO_SPRITE_H_

#include <MaterialInstance.h>
#include <Vec3.h>
#include <Vec2.h>

#include "gizmo/IGizmo.h"

namespace maj::gizmo
{
	class Sprite final : public IGizmo
	{
	public:
		static const char* skMaterialName;
		static const char* skLayoutName;

		Sprite(scene::IComponent* component, const rhi::Texture* kTexture);
		virtual ~Sprite();

		void color(const std::function<math::Vec3Holder<float>()>& kColor);
		
		virtual void enable(GizmoHandler& handler, bool enabled) override;

	private:
		virtual void draw() const override;

		void updateMaterial();
		void createMaterial();
		void resize(const math::Vec2ui& kExtent);

		rhi::MaterialInstance* mMaterialInstance;
		const rhi::Texture* mkTexture;
		rhi::Buffer::SubBuffer* mBuffer;

		std::function<math::Vec3Holder<float>()> mColor;
		math::Vec2Holder<float> mSize;

		uint32_t mCameraEventIndex;
		uint32_t mResizeIndex;
		uint32_t mSelectionId;
	};
}

#endif