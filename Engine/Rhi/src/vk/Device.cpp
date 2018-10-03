#include "Device.h"

#include <map>
#include <set>

#include "common/Logging.h"

#include "Instance.h"
#include "VkHelper.h"

namespace maj::rhi
{
	const std::vector<const char*> Device::skDeviceExtensions 
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	Device::QueueHandler::QueueHandler()
	{}

	const VkQueue& Device::QueueHandler::queue(const char* kName) const
	{
		if (mQueues.count(kName))
			return mQueues.at(kName);

		LOG(LogRHIWarning, "failed to get queue: queue \'" << kName << "\' not found")
		return mQueues.begin()->second;
	}

	int32_t Device::QueueHandler::familyIndex(const char* kName) const
	{
		if (mFamilies.count(kName))
			return mFamilies.at(kName).second;

		LOG(LogRHIWarning, "failed to get family index: queue family \'" << kName << "\' not found")
		return mFamilies.begin()->second.second;
	}

	void Device::QueueHandler::family(const char* kName, const QueueFamilyPredicate& checkFunction)
	{
		if (mFamilies.count(kName) == 0u)
		{
			mFamilies.emplace(kName, QueueFamily { checkFunction, -1 });
			return;
		}

		mFamilies[kName].first = std::move(checkFunction);
		mFamilies[kName].second = -1;
	}

	bool Device::QueueHandler::isComplete() const
	{
		for (const std::pair<const std::string, QueueFamily>& kPair : mFamilies)
			if (kPair.second.second < 0)
				return false;

		return true;
	}

	void Device::QueueHandler::fill(const Device& kDevice)
	{
		for (const std::pair<const std::string, QueueFamily>& kPair : mFamilies)
		{
			if (mQueues.count(kPair.first) == 0u)
				mQueues.emplace(kPair.first, VkQueue { VK_NULL_HANDLE });
			
			vkGetDeviceQueue(kDevice.mDevice, kPair.second.second, 0u, &mQueues[kPair.first]);
		}
	}

	void Device::QueueHandler::findFamilies(VkPhysicalDevice device)
	{
		uint32_t queueFamilyCount { 0u };
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		uint32_t i { 0u };
		for (const VkQueueFamilyProperties& kQueueFamily : queueFamilies)
		{
			for (std::pair<const std::string, QueueFamily>& pair : mFamilies)
				if(pair.second.first(device, i, kQueueFamily))
					pair.second.second = i;

			if (isComplete())
				return;

			++i;
		}
	}

	void Device::QueueHandler::resetFamilies()
	{
		for (std::pair<const std::string, QueueFamily>& pair : mFamilies)
			pair.second.second = -1;
	}

	bool Device::isDeviceSuitable(VkPhysicalDevice device, Surface& surface)
	{
		bool extensionsSupported { checkDeviceExtensionSupport(device) };
		VkPhysicalDeviceFeatures supportedFeatures {};
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		bool swapChainAdequate { false };
		if (extensionsSupported) 
		{
			Surface::Query kSwapChainSupport { surface.query(device) };
			swapChainAdequate = !kSwapChainSupport.mFormats.empty() && !kSwapChainSupport.mPresentModes.empty();
		}

		return extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount { 0u };
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions{ extensionCount };
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		LOG(LogRHIInfo, "available device extensions:")
		std::set<std::string> requiredExtensions{ skDeviceExtensions.begin(), skDeviceExtensions.end() };
		for (const VkExtensionProperties& extension : availableExtensions)
		{
			LOG(LogRHIInfo, log::Logger<>::skTab << extension.extensionName)
			requiredExtensions.erase(extension.extensionName);
		}

		if (requiredExtensions.empty())
			return true;

		LOG(LogRHIInfo, "required device extensions not supported:")
		for (const std::string& extension : requiredExtensions)
			LOG(LogRHIError, log::Logger<>::skTab << extension)

		return false;
	}

	Device::Device(Surface& surface, ExtensionList& extensions)
		: Service{ typeid(Device).name() }, mPhysicalDevice { VK_NULL_HANDLE }, mDevice { VK_NULL_HANDLE }
	{
		mQueueHandler.family("graphics", [](VkPhysicalDevice device, int32_t i, VkQueueFamilyProperties properties) -> bool
		{
			(void) device; (void) i;
			return properties.queueCount > 0u && properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
		});

		mQueueHandler.family("present", [](VkPhysicalDevice device, int32_t i, VkQueueFamilyProperties properties) -> bool
		{
			if (properties.queueCount <= 0u)
				return false;

			VkBool32 presentSupport { false };

			const std::unordered_map<uint8_t, Surface>& kSurfaces { Service::get<Instance>()->surfaces() };
			for (const std::pair<const uint8_t, Surface>& kPair : kSurfaces)
			{
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, kPair.second.surface(), &presentSupport);
				if (!presentSupport)
					return false;
			}

			return true;
		});

		const Instance* kInstance { Service::get<Instance>() };

		pickPhysicalDevice(kInstance->instance(), surface, extensions);
		createLogicalDevice(extensions);

		mQueueHandler.fill(*this);
	}

	Device::~Device()
	{
		if (mDevice)
			vkDestroyDevice(mDevice, nullptr);
	}

	void Device::updateQueues()
	{
		mQueueHandler.findFamilies(mPhysicalDevice);
		mQueueHandler.fill(*this);
	}

	VkDevice Device::device() const
	{
		return mDevice;
	}

	VkPhysicalDevice Device::physicalDevice() const
	{
		return mPhysicalDevice;
	}

	const VkPhysicalDeviceProperties& Device::physicalDeviceProperties() const
	{
		return mPhysicalDeviceProperties;
	}

	const Device::QueueHandler& Device::queueHandler() const
	{
		return mQueueHandler;
	}

	uint32_t Device::findMemoryType(uint32_t typeFilter, EMemoryFlag properties) const
	{
		VkMemoryPropertyFlags memoryFlag { static_cast<VkMemoryPropertyFlags>(toMemoryFlag(properties)) };

		for (uint32_t i { 0u } ; i < mPhysicalDeviceMemoryProperties.memoryTypeCount ; ++i)
			if (typeFilter & (1u << i) && (mPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryFlag) == memoryFlag)
				return i;

		THROW("failed to find suitable memory type");
	}

	Format Device::findSupportedFormat(const std::vector<Format>& kCandidates, EImageTiling tiling, EFormatFeatureFlag features) const
	{
		VkFormatFeatureFlags formatFeatures { static_cast<VkFormatFeatureFlags>(toFormatFeatureFlag(features)) };

		for (const Format& kFormat : kCandidates)
		{
			VkFormat format { toFormat(kFormat) };

			VkFormatProperties props {};
			vkGetPhysicalDeviceFormatProperties(mPhysicalDevice, format, &props);

			if (tiling == EImageTiling::LINEAR && (props.linearTilingFeatures & formatFeatures) == formatFeatures)
				return kFormat;

			else if (tiling == EImageTiling::OPTIMAL && (props.optimalTilingFeatures & formatFeatures) == formatFeatures)
				return kFormat;
		}

		THROW("failed to find supported format");
	}

	void Device::pickPhysicalDevice(VkInstance instance, Surface& surface, ExtensionList& extensions)
	{
		uint32_t deviceCount { 0u };
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (!deviceCount)
			THROW("failed to pick physical device: vulkan not supported")

		std::vector<VkPhysicalDevice> devices{ deviceCount };
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// Rate devices
		const auto rateDevice { [](VkPhysicalDevice device, const ExtensionList& kExtensions) -> uint32_t
		{
			uint32_t score { 0u };

			VkPhysicalDeviceProperties deviceProperties {};
			VkPhysicalDeviceFeatures deviceFeatures {};
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 1000u;

			score += deviceProperties.limits.maxImageDimension2D;

			uint32_t extensionScore { toDeviceScore(deviceFeatures, kExtensions) };
			if (extensionScore == 0u)
				score = 0u;
			else
				score += extensionScore;

			LOG(LogRHIInfo, "rating " << score << log::Logger<>::skTab << deviceProperties.deviceName)
			return score;
		} };

		std::map<uint32_t, VkPhysicalDevice> scoredDevices {};
		for (const VkPhysicalDevice& device : devices)
		{
			mQueueHandler.findFamilies(device);
			if (mQueueHandler.isComplete() && isDeviceSuitable(device, surface))
				scoredDevices.emplace(rateDevice(device, extensions), device);

			mQueueHandler.resetFamilies();
		}

		// Pick physical device with highest score
		if (!scoredDevices.empty() && scoredDevices.rbegin()->first > 0u)
			mPhysicalDevice = scoredDevices.rbegin()->second;
		else
			THROW("failed to pick physical device: no suitable GPU")

		// Update properties
		VkPhysicalDeviceFeatures deviceFeatures {};
		vkGetPhysicalDeviceFeatures(mPhysicalDevice, &deviceFeatures);
		updateExtensions(deviceFeatures, extensions);

		vkGetPhysicalDeviceProperties(mPhysicalDevice, &mPhysicalDeviceProperties);
		vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mPhysicalDeviceMemoryProperties);
	}

	void Device::createLogicalDevice(const ExtensionList& kExtensions)
	{
		mQueueHandler.findFamilies(mPhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {};
		std::set<int32_t> uniqueQueueFamilies { mQueueHandler.familyIndex("graphics"), mQueueHandler.familyIndex("present") };

		float queuePriority { 1.f };
		for (int32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1u;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.emplace_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures { toDeviceFeatures(kExtensions) };

		VkDeviceCreateInfo createInfo { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledLayerCount = 0u;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(skDeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = skDeviceExtensions.data();

		VkResult result{ vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) };
		if (result != VK_SUCCESS)
			THROW("failed to create logical device with error:" + std::to_string(result))
	}
}