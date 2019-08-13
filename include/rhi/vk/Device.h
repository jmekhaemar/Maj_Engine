#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <unordered_map>
#include <functional>

#include <Service.h>
#include "common/Format.h"
#include "common/Image.h"
#include "common/Memory.h"
#include "common/ExtensionList.h"

#include "Surface.h"

namespace maj::rhi
{
	class Device final : public Service
	{
	public:
		typedef std::function<bool (VkPhysicalDevice, int32_t, VkQueueFamilyProperties)> QueueFamilyPredicate;
		typedef std::pair<QueueFamilyPredicate, int32_t> QueueFamily;

		struct QueueHandler final : public NonCopyable
		{
		public:
			QueueHandler();
			~QueueHandler() = default;

			const VkQueue& queue(const char* kName) const;
			int32_t familyIndex(const char* kName) const;
			bool isComplete() const;

			void family(const char* kName, const QueueFamilyPredicate& checkFunction);
			void fill(const Device& kDevice);

			void findFamilies(VkPhysicalDevice device);
			void resetFamilies();

		private:
			std::unordered_map<std::string, QueueFamily> mFamilies;
			std::unordered_map<std::string, VkQueue> mQueues;
		};

	private:
		static bool isDeviceSuitable(VkPhysicalDevice device, Surface& surface);
		static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		static const std::vector<const char*> skDeviceExtensions;

	public:
		Device(Surface& surface, ExtensionList& extensions);
		~Device();

		void updateQueues();

		VkDevice device() const;
		VkPhysicalDevice physicalDevice() const;

		const VkPhysicalDeviceProperties& physicalDeviceProperties() const;
		const QueueHandler& queueHandler() const;

		uint32_t findMemoryType(uint32_t typeFilter, EMemoryFlag properties) const;
		Format findSupportedFormat(const std::vector<Format>& kCandidates, EImageTiling tiling, EFormatFeatureFlag features) const;

	private:
		void pickPhysicalDevice(VkInstance instance, Surface& surface, ExtensionList& extensions);
		void createLogicalDevice(const ExtensionList& kExtensions);

		VkPhysicalDeviceProperties mPhysicalDeviceProperties;
		VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;

		QueueHandler mQueueHandler;
	};
}

#endif