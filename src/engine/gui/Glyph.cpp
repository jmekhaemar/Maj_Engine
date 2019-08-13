#include "gui/Glyph.h"

#include <Renderer.h>

namespace maj::gui
{
	Glyph::Glyph(Info&& info, const uint8_t* bitmap)
		: mInfo { std::move(info) }, mImage { nullptr }, mTexture { nullptr }
	{
		if (bitmap)
		{
			rhi::Format format { rhi::Format::EComponent::R8, rhi::Format::EType::UNORM };

			mImage = std::make_unique<rhi::Image>(bitmap, mInfo.mSize, format);
			mTexture = std::make_unique<rhi::Texture>(mImage.get(), false);
		}
	}

	Glyph::Glyph(Glyph&& other)
		: mInfo { std::move(other.mInfo) }, mImage { std::move(other.mImage) }, mTexture { std::move(other.mTexture) }
	{}

	Glyph::~Glyph()
	{
		Service::get<rhi::Renderer>()->wait();
	}

	const Glyph::Info& Glyph::info() const
	{
		return mInfo;
	}

	const rhi::Texture* Glyph::bitmap() const
	{
		return mTexture.get();
	}
}