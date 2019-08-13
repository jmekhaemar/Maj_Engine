#include "Cursor.h"

#include <Logging.h>
#include <common/LoaderImg.h>

namespace maj::ww
{
	constexpr int32_t Cursor::convertShape(EShape shape)
	{
		switch (shape)
		{
			case EShape::ARROW:
			return GLFW_ARROW_CURSOR;

			case EShape::I_BEAM:
			return GLFW_IBEAM_CURSOR;

			case EShape::CROSSHAIR:
			return GLFW_CROSSHAIR_CURSOR;

			case EShape::HAND:
			return GLFW_HAND_CURSOR;

			case EShape::RESIZE_X:
			return GLFW_HRESIZE_CURSOR;

			case EShape::RESIZE_Y:
			return GLFW_VRESIZE_CURSOR;

			default:
			LOG(LogWarning, "undefined cursor shape")
			return GLFW_ARROW_CURSOR;
		}
	}

	Cursor::Cursor(EShape shape)
		: mCursor { glfwCreateStandardCursor(convertShape(shape)) }
	{}

	Cursor::Cursor(const char* kPath, const math::Vec2i& kHotspot)
		: mCursor { nullptr }
	{
		std::vector<uint8_t> pixels {};
		math::Vec2ui extent { 0u, 0u };
		rhi::Format format { rhi::Format::EComponent::R8G8B8A8, rhi::Format::EType::UNORM };

		try
		{
			Service::get<rhi::LoaderImg>()->load(kPath, pixels, extent, format);

			GLFWimage image { static_cast<int32_t>(extent.mX), static_cast<int32_t>(extent.mY), pixels.data() };
			mCursor = glfwCreateCursor(&image, kHotspot.mX, kHotspot.mY);
		}
		catch (const std::exception& e)
		{
			THROW("failed to create cursor: failed to load cursor \'" + std::string { kPath } + "\': " + std::string { e.what() })
		}
	}

	Cursor::~Cursor()
	{
		if (mCursor)
			glfwDestroyCursor(mCursor);
	}

	GLFWcursor* Cursor::cursor()
	{
		return mCursor;
	}
}