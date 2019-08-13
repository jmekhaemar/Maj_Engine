#include "Icon.h"

#include <Logging.h>
#include <common/LoaderImg.h>

namespace maj::ww
{
	Icon::Icon(std::vector<const char*>&& names)
		: mImages {}
	{
		uint8_t imageCount { static_cast<uint8_t>(names.size()) };
		rhi::Format format { rhi::Format::EComponent::R8G8B8A8, rhi::Format::EType::UNORM };

		mImages.reserve(imageCount);
		for (uint8_t i { 0u } ; i < imageCount ; ++i)
		{
			math::Vec2ui extent { 0u, 0u };
			std::vector<uint8_t> pixels {};

			uint8_t* data { nullptr };
			try
			{
				Service::get<rhi::LoaderImg>()->load(names[i], pixels, extent, format);
				size_t size { pixels.size() };

				data = new uint8_t[size] {};
				std::memcpy(data, pixels.data(), size);

				mImages.emplace_back(GLFWimage { static_cast<int32_t>(extent.mX), static_cast<int32_t>(extent.mY), data });
			}
			catch (const std::exception& e)
			{
				if (data)
					delete[] data;

				LOG(LogWarning, "failed to load icon \'" << names[i] << "\': " << e.what())
			}
		}
	}

	Icon::~Icon()
	{
		uint8_t imageCount { static_cast<uint8_t>(mImages.size()) };
		for (uint8_t i { 0u } ; i < imageCount ; ++i)
		{
			if (mImages[i].pixels)
				delete[] mImages[i].pixels;
		}
	}

	int32_t Icon::imageCount() const
	{
		return static_cast<int32_t>(mImages.size());
	}

	const GLFWimage* Icon::images() const
	{
		return mImages.data();
	}
}