#include "gui/Sprite.h"

#include <Renderer.h>
#include "gui/CWidget.h"

namespace maj::gui
{
	DEFINE_HASH_OVERLOAD(Sprite)

	Sprite::Sprite(CWidget* widget, const rhi::Material* kMaterial, const rhi::Texture* kTexture)
		: IElement { widget, EAnchorFlag::CENTER }, mMaterialInst { new rhi::MaterialInstance { kMaterial } }, mTexture { kTexture }, mBuffer { nullptr }
	{
		const math::Vec3ui& kImageExtent { mTexture->image()->info().mExtent };
		size({ static_cast<float>(kImageExtent.mX), static_cast<float>(kImageExtent.mY) }, gui::ECoordFlag::PIXELATED);

		initialize();
		onAttach();
	}

	Sprite::~Sprite()
	{
		if (mMaterialInst)
			delete mMaterialInst;

		rhi::Buffer::release(mBuffer);
	}

	void Sprite::initialize()
	{
		// Enable/Disable
		mOnEnabled += [this] (bool enabled) -> void
		{
			rhi::Renderer* renderer { Service::get<rhi::Renderer>() };

			if (enabled)
			{
				rhi::DrawCall::Geometry quad { { rhi::DrawCall::VertexInput { 1u, 4u } } };
				renderer->frameGraph().add(mMaterialInst, std::move(quad));
			}
			else
				renderer->frameGraph().remove(mMaterialInst);
		};

		// Buffer & Material
		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Vec2Holder<float>) * 2u + sizeof(math::Vec4Holder<float>));
		mMaterialInst->descriptor(2u, 0u, mBuffer);
		mMaterialInst->descriptor(2u, 1u, mTexture.get());
		mMaterialInst->update(2u);

		if (mIsEnabled)
		{
			rhi::Renderer* renderer { Service::get<rhi::Renderer>() };
			rhi::DrawCall::Geometry quad { { rhi::DrawCall::VertexInput { 1u, 4u } } };
			renderer->frameGraph().add(mMaterialInst, std::move(quad));
		}
	}

	void Sprite::draw() const
	{
		math::Vec2Holder<float> size { mSize.data() };
		math::Vec2Holder<float> pos { mPosition.data() };
		math::Vec4Holder<float> color { mColor.data() };

		rhi::Buffer::MemoryMap memMap { *mBuffer };
		std::memcpy(memMap.mData, &pos, sizeof(math::Vec2Holder<float>));
		std::memcpy(memMap.mData + sizeof(math::Vec2Holder<float>), &size, sizeof(math::Vec2Holder<float>));
		std::memcpy(memMap.mData + (2 * sizeof(math::Vec2Holder<float>)), &color, sizeof(math::Vec4Holder<float>));
	}

	void Sprite::camera(const rhi::IDescriptor* kBuffer)
	{
		if (mWidget->space() == ECoordSpace::WORLD)
		{
			mMaterialInst->descriptor(0u, 0u, kBuffer);
			mMaterialInst->update(0u, 0u);
		}
	}

	const rhi::Texture* Sprite::texture() const
	{
		return mTexture.get();
	}

	void Sprite::texture(const rhi::Texture* kTexture)
	{
		if (mTexture == kTexture)
			return;

		mTexture = kTexture;

		mMaterialInst->descriptor(2u, 1u, mTexture.get());
		mMaterialInst->update(2u, 1u);
	}

	void Sprite::onAttach()
	{
		IElement::onAttach();

		if (mWidget->space() == ECoordSpace::WORLD)
		{
			mMaterialInst->descriptor(1u, 0u, mWidget->buffer());
			mMaterialInst->update(1u, 0u);
		}
	}

	void Sprite::onSpaceChanged(ECoordSpace space)
	{
		if (mMaterialInst)
			delete mMaterialInst;

		const char* kMaterialName { space == ECoordSpace::SCREEN ? "asset/default/screenSpriteMaterial" : "asset/default/worldSpriteMaterial" };
		mMaterialInst = new rhi::MaterialInstance { Asset<rhi::Material>::get(kMaterialName) };
		mMaterialInst->descriptor(2u, 0u, mBuffer);
		mMaterialInst->descriptor(2u, 1u, mTexture.get());
		mMaterialInst->update(2u);

		if (space == ECoordSpace::WORLD)
		{
			mMaterialInst->descriptor(1u, 0u, mWidget->buffer());
			mMaterialInst->update(1u, 0u);
		}

		if (mIsEnabled)
		{
			rhi::Renderer* renderer { Service::get<rhi::Renderer>() };
			rhi::DrawCall::Geometry quad { { rhi::DrawCall::VertexInput { 1u, 4u } } };
			renderer->frameGraph().add(mMaterialInst, std::move(quad));
		}
	}

}