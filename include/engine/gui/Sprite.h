#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <MaterialInstance.h>

#include "gui/IElement.h"

namespace maj::gui
{
	class Sprite final : public IElement
	{
		ENABLE_META_PARENT(Sprite, IElement)

	public:
		LOAD_CONSTRUCTOR(Sprite)

		Sprite(CWidget* widget, const rhi::Material* kMaterial, const rhi::Texture* kTexture);
		virtual ~Sprite();

		virtual void draw() const override;
		virtual void camera(const rhi::IDescriptor* kBuffer) override;

		const rhi::Texture* texture() const;
		void texture(const rhi::Texture* kTexture);

	private:
		virtual void onAttach() override;
		virtual void onSpaceChanged(ECoordSpace space) override;

		rhi::MaterialInstance* mMaterialInst;
		Asset<rhi::Texture> mTexture;
		rhi::Buffer::SubBuffer* mBuffer;
	};
}

META_CLASS_PARENT(maj::gui::Sprite, maj::gui::IElement, 2u)
META_FIELD(maj::gui::Sprite, maj::rhi::MaterialInstance*, mMaterialInst, 0u)
META_FIELD(maj::gui::Sprite, Asset<rhi::Texture>, mTexture, 1u)

#endif