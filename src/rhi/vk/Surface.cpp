#include "Surface.h"

#undef max
#include <algorithm>
#include "VkHelper.h"

#include "Window.h"
#include "Instance.h"

namespace maj::rhi
{
	Surface::Query::Query(VkPhysicalDevice device, VkSurfaceKHR surface)
		: mCapabilities {}, mFormats {}, mPresentModes {}
	{
		if (!device || !surface)
			return;

		// Capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &mCapabilities);

		// Formats
		uint32_t formatCount { 0u };
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0u)
		{
			mFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, mFormats.data());
		}

		// Present Modes
		uint32_t presentModeCount { 0u };
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0u)
		{
			mPresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, mPresentModes.data());
		}
	}

	Surface::Query::Query(Query&& other)
		: mCapabilities { other.mCapabilities }, mFormats { std::move(other.mFormats) }, mPresentModes { std::move(other.mPresentModes) }
	{}

	bool Surface::Query::supports(const Format& kFormat, EColorSpace colorSpace) const
	{
		uint32_t formatCount { static_cast<uint32_t>( mFormats.size() ) };
		if (formatCount == 0u)
			return false;

		// Default format if undefined
		if (formatCount == 1u && mFormats[0u].format == VK_FORMAT_UNDEFINED)
			return true;

		VkFormat vkFormat { toFormat(kFormat) };
		VkColorSpaceKHR vkColorSpace { toColorSpace(colorSpace) };
		for (uint32_t i { 0u } ; i < formatCount ; ++i)
		{
			if (mFormats[i].format == vkFormat && mFormats[i].colorSpace == vkColorSpace)
				return true;
		}

		return false;
	}

	bool Surface::Query::supports(VkPresentModeKHR presentMode) const
	{
		uint32_t modeCount { static_cast<uint32_t>( mPresentModes.size() ) };
		if (modeCount == 0u)
			return false;

		for (uint32_t i { 0u } ; i < modeCount ; ++i)
		{
			if (mPresentModes[i] == presentMode)
				return true;
		}

		return false;
	}

	VkExtent2D Surface::Query::clamp(uint32_t width, uint32_t height) const
	{
		if (mCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return mCapabilities.currentExtent;

		VkExtent2D extent { width, height };

		extent.width = std::clamp(extent.width, mCapabilities.minImageExtent.width, mCapabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, mCapabilities.minImageExtent.height, mCapabilities.maxImageExtent.height);

		return extent;
	}

	Surface::Query& Surface::Query::operator=(Query&& other)
	{
		mCapabilities = std::move(other.mCapabilities);
		mFormats = std::move(other.mFormats);
		mPresentModes = std::move(other.mPresentModes);

		return *this;
	}

	Surface::Surface(uint8_t viewportIndex)
		: mSurface { VK_NULL_HANDLE }
	{
		VkWin32SurfaceCreateInfoKHR createInfo { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };

		const ww::Viewport& kViewport { Service::get<ww::Window>()->viewport(viewportIndex) };
		createInfo.hwnd = kViewport.handle();
		createInfo.hinstance = kViewport.handleInstance(createInfo.hwnd);

		VkInstance instance { Service::get<Instance>()->instance() };
		PFN_vkCreateWin32SurfaceKHR createWin32SurfaceKHR { reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR")) };
		if (!createWin32SurfaceKHR)
			THROW("failed to create window surface with error: " + std::to_string(VK_ERROR_EXTENSION_NOT_PRESENT))

		VkResult result { createWin32SurfaceKHR(instance, &createInfo, nullptr, &mSurface) };
		if (result != VK_SUCCESS)
			THROW("failed to create window surface with error: " + std::to_string(result))
	}

	Surface::Surface(Surface&& other)
		: mSurface { other.mSurface }
	{
		other.mSurface = VK_NULL_HANDLE;
	}

	Surface::~Surface()
	{
		if (mSurface)
			vkDestroySurfaceKHR(Service::get<Instance>()->instance(), mSurface, nullptr);
	}

	Surface::Query Surface::query(VkPhysicalDevice device) const
	{
		return { device, mSurface };
	}

	VkSurfaceKHR Surface::surface() const
	{
		return mSurface;
	}

	Surface& Surface::operator=(Surface&& other)
	{
		if (mSurface)
			vkDestroySurfaceKHR(Service::get<Instance>()->instance(), mSurface, nullptr);

		mSurface = other.mSurface;
		other.mSurface = VK_NULL_HANDLE;

		return *this;
	}
}