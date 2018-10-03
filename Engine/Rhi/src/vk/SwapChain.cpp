#include "SwapChain.h"

#include "common/Logging.h"

#include "VkHelper.h"
#include "Instance.h"

#ifdef max
#undef max
#endif

namespace maj::rhi
{
	SwapChain::Info::Info(const Surface& kSurface, const math::Vec2ui& kExtent)
		: mInfo { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR }
	{
		create(kSurface, kExtent);
	}

	SwapChain::Info::Info(Info&& other)
		: mInfo { other.mInfo }
	{}

	void SwapChain::Info::create(const Surface& kSurface, const math::Vec2ui& kExtent)
	{
		Surface::Query details { kSurface.query(Service::get<Device>()->physicalDevice()) };

		// Format and extent
		Format format { Format::EComponent::B8G8R8A8, Format::EType::UNORM };
		EColorSpace colorSpace { EColorSpace::SRGB };
		if (!details.supports(format, colorSpace))
			THROW("failed to create swap chain info: unsupported format")

		uint32_t imageCount { std::clamp(0u, details.mCapabilities.minImageCount + 1u, details.mCapabilities.maxImageCount) };

		mInfo.surface = kSurface.surface();

		mInfo.minImageCount = imageCount;
		mInfo.imageFormat = toFormat(format);
		mInfo.imageColorSpace = toColorSpace(colorSpace);
		mInfo.imageExtent = details.clamp(kExtent.mX, kExtent.mY);
		mInfo.imageArrayLayers = 1u;
		mInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// Queues image ownership
		const Device::QueueHandler& kQueueHandler { Service::get<Device>()->queueHandler() };
		uint32_t indices[] { static_cast<uint32_t>(kQueueHandler.familyIndex("graphics")), static_cast<uint32_t>(kQueueHandler.familyIndex("present")) };

		if (indices[0] != indices[1])
		{
			mInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			mInfo.queueFamilyIndexCount = 2u;
			mInfo.pQueueFamilyIndices = indices;
		}
		else
		{
			mInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			mInfo.queueFamilyIndexCount = 0u;
			mInfo.pQueueFamilyIndices = nullptr;
		}

		mInfo.preTransform = details.mCapabilities.currentTransform;
		mInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		if (details.supports(VK_PRESENT_MODE_MAILBOX_KHR))
			mInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		else if (details.supports(VK_PRESENT_MODE_IMMEDIATE_KHR))
			mInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		else
			mInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

		mInfo.clipped = VK_TRUE;
		mInfo.oldSwapchain = VK_NULL_HANDLE;
	}

	SwapChain::Info& SwapChain::Info::operator=(Info&& other)
	{
		mInfo = other.mInfo;
		return *this;
	}

	SwapChain::Semaphore::Semaphore()
		: mSemaphore { VK_NULL_HANDLE }
	{
		const Device* kDevice { Service::get<Device>() };
		VkSemaphoreCreateInfo semaphoreInfo { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

		VkResult result { vkCreateSemaphore(kDevice->device(), &semaphoreInfo, nullptr, &mSemaphore) };
		if (result != VK_SUCCESS)
			THROW("failed to create semaphore with error: " + std::to_string(result))
	}

	SwapChain::Semaphore::Semaphore(Semaphore&& other)
		: mSemaphore { other.mSemaphore }
	{
		other.mSemaphore = VK_NULL_HANDLE;
	}

	SwapChain::Semaphore::~Semaphore()
	{
		const Device* kDevice { Service::get<Device>() };

		if (mSemaphore)
			vkDestroySemaphore(kDevice->device(), mSemaphore, nullptr);
	}

	SwapChain::Semaphore& SwapChain::Semaphore::operator=(Semaphore&& other)
	{
		const Device* kDevice { Service::get<Device>() };
		if (mSemaphore)
			vkDestroySemaphore(kDevice->device(), mSemaphore, nullptr);
		
		mSemaphore = other.mSemaphore;
		other.mSemaphore = VK_NULL_HANDLE;

		return *this;
	}

	std::unordered_map<uint8_t, bool> SwapChain::present(std::unordered_map<uint8_t, SwapChain>& swapChains, Fence* submitFinished)
	{
		// Create arrays
		uint32_t swapChainCount { static_cast<uint32_t>(swapChains.size()) };
		if (swapChainCount == 0u)
			return {};

		std::unordered_map<uint8_t, bool> shouldResize {};

		std::vector<VkSwapchainKHR> swapBuffers {};
		swapBuffers.reserve(swapChainCount);

		std::vector<uint32_t> imageIndices {};
		imageIndices.reserve(swapChainCount);

		std::vector<VkCommandBuffer> commandBuffers {};
		commandBuffers.reserve(swapChainCount);

		std::vector<VkSemaphore> imageSemaphores {};
		imageSemaphores.reserve(swapChainCount);

		std::vector<VkSemaphore> submitSemaphores {};
		submitSemaphores.reserve(swapChainCount);

		const Device* kDevice { Service::get<Device>() };
		vkQueueWaitIdle(kDevice->queueHandler().queue("present"));

		// Fill arrays
		uint32_t imageIndex { 0u };
		for (const std::pair<const uint8_t, SwapChain>& kPair : swapChains)
		{
			VkResult result { vkAcquireNextImageKHR(kDevice->device(), kPair.second.mSwapChain, std::numeric_limits<uint64_t>::max(),
													kPair.second.mImageAvailable.mSemaphore, VK_NULL_HANDLE, &imageIndex) };

			if (result == VK_ERROR_OUT_OF_DATE_KHR)
				shouldResize.emplace(kPair.first, true);

			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				THROW("failed to acquire next image with error: " + std::to_string(result))

			else
				shouldResize.emplace(kPair.first, false);

			swapBuffers.emplace_back(kPair.second.mSwapChain);
			imageIndices.emplace_back(imageIndex);
			commandBuffers.emplace_back(kPair.second.mFrames[imageIndex].commandBuffer());
			imageSemaphores.emplace_back(kPair.second.mImageAvailable.mSemaphore);
			submitSemaphores.emplace_back(kPair.second.mRenderFinished.mSemaphore);
		}

		// Fill additional arrays
		uint32_t imagesCount { static_cast<uint32_t>(imageSemaphores.size()) };
		std::vector<VkPipelineStageFlags> waitStages {};
		waitStages.resize(imagesCount, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		std::vector<VkResult> results { imagesCount, VK_SUCCESS };

		// Submit
		VkSubmitInfo submitInfo { VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.waitSemaphoreCount = imagesCount;
		submitInfo.pWaitSemaphores = imageSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages.data();

		submitInfo.commandBufferCount = imagesCount;
		submitInfo.pCommandBuffers = commandBuffers.data();

		submitInfo.signalSemaphoreCount = imagesCount;
		submitInfo.pSignalSemaphores = submitSemaphores.data();

		if (submitFinished)
			submitFinished->reset();

		VkResult result { vkQueueSubmit(kDevice->queueHandler().queue("graphics"), 1u, &submitInfo, submitFinished ? submitFinished->fence() : VK_NULL_HANDLE) };
		if (result != VK_SUCCESS)
			THROW("failed to submit to graphics queue with error: " + std::to_string(result))

		if (submitFinished)
			submitFinished->validate();

		// Present
		VkPresentInfoKHR presentInfo { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
		presentInfo.waitSemaphoreCount = imagesCount;
		presentInfo.pWaitSemaphores = submitSemaphores.data();

		presentInfo.swapchainCount = imagesCount;
		presentInfo.pSwapchains = swapBuffers.data();
		presentInfo.pImageIndices = imageIndices.data();
		presentInfo.pResults = results.data();

		vkQueuePresentKHR(kDevice->queueHandler().queue("present"), &presentInfo);

		// Check result
		uint8_t i { 0u };
		for (std::unordered_map<uint8_t, bool>::iterator it { shouldResize.begin() } ; it != shouldResize.end() && i < imagesCount ; ++it)
		{
			if (it->second || results[i] == VK_SUCCESS)
			{
				++i;
				continue;
			}

			if (results[i] == VK_ERROR_OUT_OF_DATE_KHR || results[i] == VK_SUBOPTIMAL_KHR)
				it->second = true;
			else
				THROW("failed to present image " + std::to_string(i) + " with error: " + std::to_string(result))

			++i;
		}

		return shouldResize;
	}

	SwapChain::SwapChain(Info&& info, RenderPass::Info&& renderPassInfo)
		: mInfo { std::move(info) }, mRenderPass { std::move(renderPassInfo) }, mSwapChain { VK_NULL_HANDLE }, mImageAvailable {}, mRenderFinished {}, mImages {},
		mInputAttachments {}, mFrames {}, mRecordEventIndex { 0u }
	{
		createSwapChain();
		createImages();
		createFrames();

		FrameGraph& frameGraph { Service::get<Renderer>()->frameGraph() };
		mRecordEventIndex = frameGraph.mOnRecorded + [this, &frameGraph] () -> void
		{
			record(frameGraph);
		};
		record(frameGraph);
	}

	SwapChain::SwapChain(SwapChain&& other)
		: mInfo { std::move(other.mInfo) }, mRenderPass { std::move(other.mRenderPass) }, mSwapChain { other.mSwapChain }, mImageAvailable { std::move(other.mImageAvailable) },
		mRenderFinished { std::move(other.mRenderFinished) }, mImages { std::move(other.mImages) }, mInputAttachments { std::move(other.mInputAttachments) },
		mFrames { std::move(other.mFrames) }, mRecordEventIndex { 0u }
	{
		other.mSwapChain = VK_NULL_HANDLE;

		FrameGraph& frameGraph { Service::get<Renderer>()->frameGraph() };
		mRecordEventIndex = frameGraph.mOnRecorded + [this, &frameGraph] () -> void
		{
			record(frameGraph);
		};
	}

	SwapChain::~SwapChain()
	{
		FrameGraph& frameGraph { Service::get<Renderer>()->frameGraph() };
		frameGraph.mOnRecorded -= mRecordEventIndex;

		const Device* kDevice { Service::get<Device>() };
		vkDeviceWaitIdle(kDevice->device());

		if (mSwapChain)
			vkDestroySwapchainKHR(kDevice->device(), mSwapChain, nullptr);
	}

	void SwapChain::resize(uint8_t viewportIndex, const math::Vec2ui& kExtent)
	{
		const Device* kDevice { Service::get<Device>() };
		vkDeviceWaitIdle(kDevice->device());

		// Clean previous swap chain
		if (mSwapChain)
			vkDestroySwapchainKHR(kDevice->device(), mSwapChain, nullptr);

		mFrames.clear();
		mInputAttachments.clear();
		mImages.clear();

		// Change info
		mInfo.create(Service::get<Instance>()->surface(viewportIndex), kExtent);

		// Recreate swap chain with new info
		createSwapChain();
		createImages();
		createFrames();
	}

	RenderPass& SwapChain::renderPass()
	{
		return mRenderPass;
	}

	const RenderPass& SwapChain::renderPass() const
	{
		return mRenderPass;
	}

	const InputAttachment* SwapChain::inputAttachment(EAttachmentUsageFlag usage, uint32_t index) const
	{
		usage |= EAttachmentUsageFlag::INPUT;
		if (mInputAttachments.count(usage) == 0u)
		{
			LOG(LogRHIWarning, "failed to retrieve input attachment: no attachment with usage " << static_cast<uint8_t>(usage))
			return nullptr;
		}

		const std::vector<InputAttachment>& kAttachments { mInputAttachments.at(usage) };
		uint32_t count { static_cast<uint32_t>(kAttachments.size()) };
		if (index >= count)
		{
			LOG(LogRHIWarning, "failed to retrieve input attachment: index out of range")
			return nullptr;
		}

		return &kAttachments[index];
	}

	SwapChain& SwapChain::operator=(SwapChain&& other)
	{
		const Device* kDevice { Service::get<Device>() };
		vkDeviceWaitIdle(kDevice->device());

		if (mSwapChain)
			vkDestroySwapchainKHR(kDevice->device(), mSwapChain, nullptr);

		mInfo = std::move(other.mInfo);
		mRenderPass = std::move(other.mRenderPass);

		mSwapChain = other.mSwapChain;
		other.mSwapChain = VK_NULL_HANDLE;

		mImageAvailable = std::move(other.mImageAvailable);
		mRenderFinished = std::move(other.mRenderFinished);

		mImages = std::move(other.mImages);
		mInputAttachments = std::move(other.mInputAttachments);
		mFrames = std::move(other.mFrames);

		return *this;
	}

	void SwapChain::createSwapChain()
	{
		const Device* kDevice { Service::get<Device>() };

		VkResult result { vkCreateSwapchainKHR(kDevice->device(), &mInfo.mInfo, nullptr, &mSwapChain) };
		if (result != VK_SUCCESS)
			THROW("failed to create swapchain with error: " + std::to_string(result));
	}

	Image::ImgInfo SwapChain::createImageInfo(const AttachmentInfo& kAttachment) const
	{
		const AttachmentDescription& kDescription { kAttachment.description() };

		Image::ImgInfo info {};
		info.mFormatComponent = kDescription.mFormat.mComponent;
		info.mFormatType = kDescription.mFormat.mType;
		info.mExtent = { mInfo.mInfo.imageExtent.width, mInfo.mInfo.imageExtent.height, 1u };
		info.mArrayLayers = mInfo.mInfo.imageArrayLayers;
		info.mSamples = kDescription.mSamples;
		info.mUsage = toImageUsage(kAttachment.usage());
		info.mMemory = EMemoryFlag::DEVICE_LOCAL;

		std::vector<uint32_t> queueFamilies {};
		queueFamilies.reserve(mInfo.mInfo.queueFamilyIndexCount);
		std::memcpy(queueFamilies.data(), mInfo.mInfo.pQueueFamilyIndices, mInfo.mInfo.queueFamilyIndexCount);

		info.mQueueFamilies = std::move(queueFamilies);
		info.mLayout = kDescription.mInitialLayout;

		return info;
	}

	void SwapChain::createImages()
	{
		// Attachments form the render pass
		const std::vector<AttachmentInfo>& kAttachments { mRenderPass.attachments() };
		uint32_t attachmentCount { static_cast<uint32_t>(kAttachments.size()) };
		if (attachmentCount == 0u)
		{
			LOG(LogRHIWarning, "failed to create swap chain images: no attachment in render pass")
			return;
		}

		// Color attachment from the swap chain
		const Device* kDevice { Service::get<Device>() };
		uint32_t swapImageCount { 0u };
		vkGetSwapchainImagesKHR(kDevice->device(), mSwapChain, &swapImageCount, nullptr);
		
		if (swapImageCount == 0u)
		{
			LOG(LogRHIWarning, "failed to create swap chain images: no image in swap chain")
			return;
		}

		std::vector<VkImage> swapImages {};
		swapImages.resize(swapImageCount, VK_NULL_HANDLE);
		vkGetSwapchainImagesKHR(kDevice->device(), mSwapChain, &swapImageCount, swapImages.data());

		// Layouts for transition
		std::unordered_map<EAttachmentUsageFlag, std::vector<EImageLayout>> layouts {};

		// For each attachment in the render pass
		for (uint32_t i { 0u } ; i < attachmentCount ; ++i)
		{
			const AttachmentDescription& kDescription { kAttachments[i].description() };
			EAttachmentUsageFlag usage { kAttachments[i].usage() };
			
			// If we don't have this usage
			if (mImages.count(usage) == 0u)
			{
				uint32_t usageCount { mRenderPass.countAttachments(usage) };
				mImages.emplace(usage, std::vector<Image> {});

				// Color (only) images don't need layout transition, and there needs to be one by frame
				if (usage != EAttachmentUsageFlag::COLOR)
				{
					layouts.emplace(usage, std::vector<EImageLayout> {});
					layouts.at(usage).reserve(usageCount);
					mImages.at(usage).reserve(usageCount);
				}
				else
					mImages.at(usage).reserve(usageCount * swapImageCount);
			}

			// If this is an color attachment of the swap chain
			if (usage == EAttachmentUsageFlag::COLOR)
			{
				for (uint32_t j { 0u } ; j < swapImageCount ; ++j)
					mImages.at(usage).emplace_back(createImageInfo(kAttachments[i]), swapImages[j]);
			}
			else
			{
				mImages.at(usage).emplace_back(createImageInfo(kAttachments[i]));
				layouts.at(usage).emplace_back(kDescription.mFinalLayout);
			}
		}

		// Transition image layouts & create attachments

		for (std::unordered_map<EAttachmentUsageFlag, std::vector<EImageLayout>>::iterator it { layouts.begin() } ; it != layouts.end() ; ++it)
		{
			std::vector<Image>& images { mImages.at(it->first) };
			Image::transitionLayout(images, it->second);

			// Input images have corresponding input attachments for materials to reference
			if ((it->first & EAttachmentUsageFlag::INPUT) == EAttachmentUsageFlag::INPUT)
			{
				std::vector<InputAttachment> inputAttachments {};
				uint32_t imageCount { static_cast<uint32_t>(images.size()) };
				inputAttachments.reserve(imageCount);

				for (uint32_t i { 0u } ; i < imageCount ; ++i)
					inputAttachments.emplace_back(images[i]);

				mInputAttachments.emplace(it->first, std::move(inputAttachments));
			}
		}
	}

	void SwapChain::createFrames()
	{
		const std::vector<AttachmentInfo>& kAttachments { mRenderPass.attachments() };
		uint32_t attachmentCount { static_cast<uint32_t>(kAttachments.size()) };

		const std::vector<Image>& swapImages { mImages.at(EAttachmentUsageFlag::COLOR) };
		uint32_t swapImageCount { static_cast<uint32_t>(swapImages.size()) };
		mFrames.reserve(swapImageCount);

		const math::Vec2ui kExtent { mInfo.mInfo.imageExtent.width, mInfo.mInfo.imageExtent.height };
		for (uint32_t i { 0u }; i < swapImageCount; ++i)
		{
			std::vector<const Image*> attachmentImages {};
			attachmentImages.reserve(attachmentCount);

			std::unordered_map<EAttachmentUsageFlag, uint32_t> counts {};

			for (uint32_t j { 0u } ; j < attachmentCount ; ++j)
			{
				EAttachmentUsageFlag usage { kAttachments[j].usage() };

				if (counts.count(usage) == 0u)
					counts.emplace(usage, 0u);

				uint32_t id { counts.at(usage)++ };
				if (usage != EAttachmentUsageFlag::COLOR)
					attachmentImages.emplace_back(&mImages.at(usage)[id]);
				else
					attachmentImages.emplace_back(&mImages.at(usage)[id * swapImageCount + i]);
			}

			mFrames.emplace_back(mRenderPass, attachmentImages, kExtent);
		}
	}

	void SwapChain::record(const FrameGraph& kFrameGraph) const
	{
		uint32_t frameCount { static_cast<uint32_t>(mFrames.size()) };

		for (uint32_t i { 0u } ; i < frameCount ; ++i)
			mFrames[i].record(mRenderPass, kFrameGraph);
	}
}