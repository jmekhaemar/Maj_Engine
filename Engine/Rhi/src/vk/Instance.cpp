#include "Instance.h"

#include <Window.h>

#include "common/Logging.h"

namespace maj::rhi
{
	const std::vector<const char*> Instance::skValidationLayers
	{ 
		"VK_LAYER_LUNARG_standard_validation"
	};

#ifdef _DEBUG
	const bool Instance::skEnableValidationLayers{ true };
#else
	const bool Instance::skEnableValidationLayers{ false };
#endif

	Instance::Context::Context()
		: mInstance { VK_NULL_HANDLE }, mDebugCallback { VK_NULL_HANDLE }, mSurfaces {}
	{
		createInstance();

		// Creating debug callback
		if constexpr (skEnableValidationLayers)
		{
			VkDebugReportCallbackCreateInfoEXT debugCreateInfo { VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT };
			debugCreateInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
				VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_ERROR_BIT_EXT |
				VK_DEBUG_REPORT_DEBUG_BIT_EXT;

			debugCreateInfo.pfnCallback = debugCallback;

			VkResult result { createDebugCallback(mInstance, &debugCreateInfo, &mDebugCallback) };
			if (result != VK_SUCCESS)
				THROW("failed to set up debug callback with error: " + std::to_string(result))
		}

		mSurfaces.emplace(static_cast<uint8_t>(0u), static_cast<uint8_t>(0u));
	}

	Instance::Context::~Context()
	{
		if (!mInstance)
			return;

		mSurfaces.clear();

		if constexpr (skEnableValidationLayers)
			destroyDebugCallback(mInstance, mDebugCallback);

		vkDestroyInstance(mInstance, nullptr);
	}

	void Instance::Context::createInstance()
	{
		if constexpr (skEnableValidationLayers)
		{
			if (!checkValidationLayerSupport())
				THROW("failed to create instance context: validation layers requested but not available")
		}

		// Application information & version
		VkApplicationInfo appInfo { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		appInfo.pApplicationName = "Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "MAJ";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		createInfo.pApplicationInfo = &appInfo;

		// Validation layers
		if constexpr (skEnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(skValidationLayers.size());
			createInfo.ppEnabledLayerNames = skValidationLayers.data();
		}
		else
			createInfo.enabledLayerCount = 0u;

		// Extensions check & log
		uint32_t windowExtensionsCount { 0u };
		const char** windowExtensions { Service::get<ww::Window>()->extensionsRequired(windowExtensionsCount) };

		std::vector<const char*> extensions { windowExtensions, windowExtensions + windowExtensionsCount };
		if constexpr (skEnableValidationLayers)
			extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		uint32_t extensionCount { 0u };
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions { extensionCount };
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

		LOG(LogRHIInfo, "available instance extensions:")
		for (const VkExtensionProperties& extension : availableExtensions)
			LOG(LogRHIInfo, log::Logger<>::skTab << extension.extensionName)

		const auto isAvailable
		{
			[&availableExtensions](const std::string extension) -> bool
			{
				for (const VkExtensionProperties& ext : availableExtensions)
					if (extension == ext.extensionName)
						return true;

				return false;
			}
		};

		LOG(LogRHIInfo, "required instance extensions not supported:")
		for (uint32_t i { 0u }; i < windowExtensionsCount; ++i)
		{
			if (!isAvailable(windowExtensions[i]))
				LOG(LogRHIError, log::Logger<>::skTab << windowExtensions[i])
		}

		// Creating the instance
		VkResult result { vkCreateInstance(&createInfo, nullptr, &mInstance) };
		if (result != VK_SUCCESS)
			THROW("failed to create instance with error: " + std::to_string(result))
	}

	VkResult Instance::createDebugCallback(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* kCreateInfo, VkDebugReportCallbackEXT* callback)
	{
		PFN_vkCreateDebugReportCallbackEXT func { reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance,
																									   "vkCreateDebugReportCallbackEXT")) };

		if (func)
			return func(instance, kCreateInfo, nullptr, callback);

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void Instance::destroyDebugCallback(VkInstance instance, VkDebugReportCallbackEXT callback)
	{
		PFN_vkDestroyDebugReportCallbackEXT func { reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance,
																										 "vkDestroyDebugReportCallbackEXT")) };
		if (func)
			func(instance, callback, nullptr);
	}

	bool Instance::checkValidationLayerSupport()
	{
		uint32_t layerCount { 0u };
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers { layerCount };
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : skValidationLayers)
		{
			bool layerFound { false };
			for (const VkLayerProperties& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}
			if (!layerFound)
				return false;
		}

		return true;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,
														   const char* kLayerPrefix, const char* kMsg, void* userData)
	{
		(void) kLayerPrefix;
		(void) code;
		(void) userData;
		(void) location;
		(void) obj;
		(void) objType;

		switch (flags)
		{
			case VK_DEBUG_REPORT_ERROR_BIT_EXT:
			LOG(LogRHIError, "validation layer: " << kMsg)
			break;

			case VK_DEBUG_REPORT_WARNING_BIT_EXT:
			LOG(LogRHIWarning, "validation layer: " << kMsg)
			break;

			case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
			LOG(LogRHIWarning, "validation layer: " << kMsg)
			break;

			case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
			LOG(LogRHIInfo, "validation layer: " << kMsg)
			break;

			default:
			LOG(LogRHIDebug, "validation layer: " << kMsg)
			break;
		}

		return VK_FALSE;
	}

	Instance::Instance(const ExtensionList& kExtensions)
		: Service { typeid(Instance).name() }, mExtensions { kExtensions }, mContext {}, mDevice { mContext.mSurfaces.at(0u), mExtensions }, mMemoryAllocator {},
		mRenderer {}
	{}

	void Instance::update()
	{
		if (Service::get<ww::Input>()->key(ww::EKey::F10).isPressed())
			mMemoryAllocator.dump();
	}

	void Instance::draw()
	{
		mRenderer.draw();
	}

	void Instance::addSurface(uint8_t viewportIndex)
	{
		if (mExtensions.mMultiViewport == EExtensionLevel::DISABLED)
			THROW("missing device extension: multiple viewport not enabled")

		mContext.mSurfaces.emplace(viewportIndex, viewportIndex);
		mDevice.updateQueues();

		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(viewportIndex) };
		viewport.mOnClosed += [this] (uint8_t id) -> void
		{
			mRenderer.removeSwapChain(id);
			mContext.mSurfaces.erase(id);
		};

		mRenderer.addSwapChain(viewportIndex);
	}

	const ExtensionList& Instance::extensions() const
	{
		return mExtensions;
	}

	const VkInstance& Instance::instance() const
	{
		return mContext.mInstance;
	}

	const std::unordered_map<uint8_t, Surface>& Instance::surfaces() const
	{
		return mContext.mSurfaces;
	}

	const Surface& Instance::surface(uint8_t i) const
	{
		if (mContext.mSurfaces.count(i) == 0u)
		{
			LOG(LogRHIWarning, "failed to retrieve surface: unknown index")
			return mContext.mSurfaces.at(0u);
		}

		return mContext.mSurfaces.at(i);
	}
}
