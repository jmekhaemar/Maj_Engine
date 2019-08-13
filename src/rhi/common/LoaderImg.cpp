#include "common/LoaderImg.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace maj::rhi
{
	LoaderImg::LoaderImg()
		: Service { typeid(LoaderImg).name() }
	{}

	void LoaderImg::load(const char* kPath, std::vector<uint8_t>& data, math::Vec2ui& extent, const rhi::Format& kImgFormat)
	{
		// Check if the format is readable by stb_image
		// stb_image only reads grey (R8), grey_alpha (no equivalent), rgb (R8G8B8) or rgb_alpha (R8G8B8A8)
		// Other formats would require a conversion
		uint8_t stbFormat { 0u };
		switch (kImgFormat.mComponent)
		{
			case rhi::Format::EComponent::R8:
			stbFormat = STBI_grey;
			break;

			case rhi::Format::EComponent::R8G8B8:
			stbFormat = STBI_rgb;
			break;

			case rhi::Format::EComponent::R8G8B8A8:
			stbFormat = STBI_rgb_alpha;
			break;

			default:
			THROW("format component not supported by stb_image")
		}

		int32_t texWidth { 0 }, texHeight { 0 }, texChannels { 0 };
		stbi_uc* pixels { stbi_load(kPath, &texWidth, &texHeight, &texChannels, stbFormat) };
		if (!pixels)
			THROW("can't open file \'" + std::string { kPath } + '\'')

		size_t imageSize { static_cast<size_t>(texWidth * texHeight * stbFormat) };
		data.resize(imageSize, 0u);

		std::memcpy(data.data(), pixels, imageSize);

		stbi_image_free(pixels);

		extent.mX = static_cast<uint32_t>(texWidth);
		extent.mY = static_cast<uint32_t>(texHeight);
	}
}