#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>

#include <NonCopyable.h>
#include <Service.h>
#include "common/ExtensionList.h"

#include "Device.h"
#include "Renderer.h"
#include "MemoryAllocator.h"

namespace maj::rhi
{
	class Instance final : public Service
	{
	private:
		struct Context final : public NonCopyable
		{
			Context();
			~Context();

			void createInstance();

			VkInstance mInstance;
			VkDebugReportCallbackEXT mDebugCallback;
			std::unordered_map<uint8_t, Surface> mSurfaces;
		};

	private:
		static VkResult createDebugCallback(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* kCreateInfo,	VkDebugReportCallbackEXT* callback);
		static void destroyDebugCallback(VkInstance instance, VkDebugReportCallbackEXT callback);

		static bool checkValidationLayerSupport();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
															uint64_t obj, size_t location, int32_t code, const char* kLayerPrefix, const char* kMsg, void* userData);

		static const std::vector<const char*> skValidationLayers;
		static const bool skEnableValidationLayers;

	public:
		Instance(const ExtensionList& kExtensions);
		~Instance() = default;

		void update();
		void draw();

		void addSurface(uint8_t viewportIndex);

		const ExtensionList& extensions() const;
		const VkInstance& instance() const;

		const std::unordered_map<uint8_t, Surface>& surfaces() const;
		const Surface& surface(uint8_t i) const;

	private:
		ExtensionList mExtensions;

		Context mContext;
		Device mDevice;

		MemoryAllocator mMemoryAllocator;

		Renderer mRenderer;
	};
}

#endif