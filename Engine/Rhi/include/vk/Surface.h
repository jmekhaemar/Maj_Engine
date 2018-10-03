#ifndef _SURFACE_H_
#define _SURFACE_H_

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.h>
#include <vector>

#include <NonCopyable.h>
#include "common/Format.h"

namespace maj::rhi
{
	class Surface final : public NonCopyable
	{
	public:
		struct Query final : public NonCopyable
		{
			Query(VkPhysicalDevice device, VkSurfaceKHR surface);
			Query(Query&& other);
			~Query() = default;

			bool supports(const Format& kFormat, EColorSpace colorSpace) const;
			bool supports(VkPresentModeKHR presentMode) const;
			VkExtent2D clamp(uint32_t width, uint32_t height) const;

			Query& operator=(Query&& other);

			VkSurfaceCapabilitiesKHR mCapabilities;
			std::vector<VkSurfaceFormatKHR> mFormats;
			std::vector<VkPresentModeKHR> mPresentModes;
		};

		Surface(uint8_t viewportIndex);
		Surface(Surface&& other);
		~Surface();

		Query query(VkPhysicalDevice device) const;
		VkSurfaceKHR surface() const;

		Surface& operator=(Surface&& other);

	private:
		VkSurfaceKHR mSurface;
	};
}

#endif