#include "Image.h"

#include <sstream>
#include <algorithm>

#include "common/LoaderImg.h"
#include "common/Logging.h"

#include "VkHelper.h"
#include "Instance.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(Image::ImgInfo)
	DEFINE_HASH_OVERLOAD(Image)

	Image::ImgInfo::ImgInfo()
		: mType{ EImageType::SINGLE_2D }, mFormatComponent{ Format::EComponent::UNDEFINED }, mFormatType{ Format::EType::UNORM }, mExtent{}, mMipLevels{ 1u }, mArrayLayers{ 1u },
		mSamples { ESampleFlag::COUNT_1 }, mTiling { EImageTiling::OPTIMAL }, mUsage { EImageUsageFlag::SAMPLED }, mQueueFamilies {}, mLayout { EImageLayout::UNDEFINED },
		mMemory { EMemoryFlag::DEVICE_LOCAL }
	{}

	void Image::ImgInfo::initialize()
	{}

	VkImageCreateInfo Image::ImgInfo::imageInfo() const
	{
		VkImageCreateInfo imageInfo { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		if (mType == EImageType::CUBE || mType == EImageType::CUBE_ARRAY)
			imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		if (mType == EImageType::CUBE_ARRAY && kExtensions.mCubeArray == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: cube array not enabled")
			imageInfo.arrayLayers = 1u;
		}
		else
			imageInfo.arrayLayers = mArrayLayers;

		imageInfo.imageType = toImageType(mType);
		imageInfo.format = toFormat(format());
		imageInfo.extent = { mExtent.mX, mExtent.mY, mExtent.mZ };
		imageInfo.mipLevels = mMipLevels;
		imageInfo.samples = toSampleFlag(mSamples);
		imageInfo.tiling = toImageTiling(mTiling);
		imageInfo.usage = toImageUsage(mUsage);

		uint32_t queueFamilyCount { static_cast<uint32_t>(mQueueFamilies.size()) };
		if (queueFamilyCount > 1u)
		{
			imageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
			imageInfo.queueFamilyIndexCount = queueFamilyCount;
			imageInfo.pQueueFamilyIndices = mQueueFamilies.data();
		}
		else
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		imageInfo.initialLayout = toImageLayout(mLayout);

		return imageInfo;
	}

	VkImageViewCreateInfo Image::ImgInfo::viewInfo(VkImage image) const
	{
		VkImageViewCreateInfo viewInfo { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		viewInfo.image = image;
		viewInfo.viewType = toImageViewType(mType);
		viewInfo.format = toFormat(format());
		viewInfo.subresourceRange.aspectMask = toImageAspect(mUsage);
		viewInfo.subresourceRange.baseMipLevel = 0u;
		viewInfo.subresourceRange.levelCount = mMipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0u;

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		if (mType == EImageType::CUBE_ARRAY && kExtensions.mCubeArray == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: cube array not enabled")
			viewInfo.subresourceRange.layerCount = 1u;
		}
		else
			viewInfo.subresourceRange.layerCount = mArrayLayers;

		return viewInfo;
	}

	Format Image::ImgInfo::format() const
	{
		return { mFormatComponent, mFormatType };
	}

	Image::MemoryMap::MemoryMap(const Image& kImage)
		: mData { nullptr }, mRowStride { 0u }, mMemoryInfo { kImage.mMemoryInfo }
	{
		if (!mMemoryInfo.mMemoryChunk)
			return;

		const Device* kDevice { Service::get<Device>() };

		// Retrieve informations
		VkImageSubresource subResource {};
		subResource.aspectMask = toImageAspect(kImage.mInfo.mUsage);
		VkSubresourceLayout subResourceLayout {};
		vkGetImageSubresourceLayout(kDevice->device(), kImage.mImage, &subResource, &subResourceLayout);

		mRowStride = subResourceLayout.rowPitch;

		// Map
		VkResult result { vkMapMemory(kDevice->device(), mMemoryInfo.mMemoryChunk->memory(), mMemoryInfo.mMemoryBlock->memoryOffset() + subResourceLayout.offset,
									  subResourceLayout.size, 0u, reinterpret_cast<void**>(&mData)) };
		if (result != VK_SUCCESS)
			THROW("failed to map memory with error: " + std::to_string(result))
	}

	Image::MemoryMap::~MemoryMap()
	{
		if (!mMemoryInfo.mMemoryChunk)
			return;

		vkUnmapMemory(Service::get<Device>()->device(), mMemoryInfo.mMemoryChunk->memory());
	}

	void Image::transitionLayout(std::vector<Image>& images, EImageLayout newLayout)
	{
		Renderer::SingleTimeCommand commandBuffer {};

		uint32_t imageCount { static_cast<uint32_t>(images.size()) };
		for (uint32_t i { 0u } ; i < imageCount ; ++i)
			images[i].recordLayoutTransition(commandBuffer.mBuffer, newLayout);
	}

	void Image::transitionLayout(std::vector<Image>& images, const std::vector<EImageLayout>& kNewLayouts)
	{
		// Check array size
		uint32_t imageCount { static_cast<uint32_t>(images.size()) };
		uint32_t layoutCount { static_cast<uint32_t>(kNewLayouts.size()) };
		
		if (imageCount != layoutCount)
			THROW("failed to transition image layout: image array and layout array have different size")

		if (layoutCount == 0u)
			THROW("failed to transition image layout: empty layout array")

		// Record
		Renderer::SingleTimeCommand commandBuffer {};

		for (uint32_t i { 0u } ; i < imageCount ; ++i)
		{
			EImageLayout layout { i < layoutCount ? kNewLayouts[i] : kNewLayouts[layoutCount - 1u] };
			images[i].recordLayoutTransition(commandBuffer.mBuffer, layout);
		}
	}

	void Image::copy(const Image& kSrc, Image& dst, EImageLayout dstLayout)
	{
		if (kSrc.mInfo.mLayout != EImageLayout::TRANSFER_SRC)
		{
			LOG(LogRHIWarning, "failed to copy image: source image is not in layout TRANSFER_SRC")
			return;
		}

		VkImageCopy region {};
		region.srcSubresource.aspectMask = toImageAspect(kSrc.mInfo.mUsage);
		region.srcSubresource.baseArrayLayer = 0u;
		region.srcSubresource.layerCount = kSrc.mInfo.mArrayLayers;
		region.srcSubresource.mipLevel = 0u;

		region.dstSubresource.aspectMask = toImageAspect(dst.mInfo.mUsage);
		region.dstSubresource.baseArrayLayer = 0u;
		region.dstSubresource.layerCount = dst.mInfo.mArrayLayers;
		region.dstSubresource.mipLevel = 0u;

		region.extent = { std::min(kSrc.mInfo.mExtent.mX, dst.mInfo.mExtent.mX), std::min(kSrc.mInfo.mExtent.mY, dst.mInfo.mExtent.mY), 1u };

		if (region.srcSubresource.aspectMask != region.dstSubresource.aspectMask)
		{
			LOG(LogRHIWarning, "failed to copy image: images' aspects don't match")
			return;
		}

		Renderer::SingleTimeCommand commandBuffer {};
		dst.recordLayoutTransition(commandBuffer.mBuffer, EImageLayout::TRANSFER_DST);
		vkCmdCopyImage(commandBuffer.mBuffer, kSrc.mImage, toImageLayout(EImageLayout::TRANSFER_SRC), dst.mImage, toImageLayout(EImageLayout::TRANSFER_DST), 1u, &region);
		dst.recordLayoutTransition(commandBuffer.mBuffer, dstLayout);
	}

	Image::Image(JsonNode* root)
		: mkPath {}
	{
		load(root);
		initialize();
	}

	Image::Image(const char* kPath, const Format& kFormat, EImageType type, EMemoryFlag properties)
		: mkPath { kPath }, mInfo {}, mImage{ VK_NULL_HANDLE }, mView{ VK_NULL_HANDLE }, mMemoryInfo {}
	{
		mInfo.mFormatComponent = kFormat.mComponent;
		mInfo.mFormatType = kFormat.mType;
		mInfo.mType = type;
		mInfo.mMemory = properties;

		initialize();
	}
	
	Image::Image(const char* kPath[6u], const Format& kFormat, EMemoryFlag properties)
		: mkPath {	std::string { kPath[0u] } + ';' + std::string { kPath[1u] } + ';' + std::string { kPath[2u] } + ';' + std::string { kPath[3u] } + ';' + std::string { kPath[4u] } +
					';' + std::string { kPath[5u] } },
		mInfo{}, mImage{ VK_NULL_HANDLE }, mView{ VK_NULL_HANDLE }, mMemoryInfo {}
	{
		mInfo.mFormatComponent = kFormat.mComponent;
		mInfo.mFormatType = kFormat.mType;
		mInfo.mType = EImageType::CUBE;
		mInfo.mArrayLayers = 6u;
		mInfo.mMemory = properties;

		initialize();
	}

	Image::Image(const uint8_t* kImage, const math::Vec2ui& kExtent, const Format& kFormat, EMemoryFlag properties)
		: mkPath {}, mInfo {}, mImage { VK_NULL_HANDLE }, mView { VK_NULL_HANDLE }, mMemoryInfo {}
	{
		if (!kImage || kExtent.mX == 0u || kExtent.mY == 0u)
			return;

		mInfo.mMemory = properties;

		initialize(kImage, kExtent, kFormat);
	}

	Image::Image(const ImgInfo& kInfo)
		: mkPath {}, mInfo { kInfo }, mImage { VK_NULL_HANDLE }, mView { VK_NULL_HANDLE }
	{
		createImage();
		createView();
	}

	Image::Image(const ImgInfo& kInfo, VkImage image)
		: mkPath {}, mInfo { kInfo }, mImage { image }, mView { VK_NULL_HANDLE }, mMemoryInfo { 0u, nullptr, nullptr }
	{
		createView();
	}

	Image::Image(Image&& other)
		: mkPath { std::move(other.mkPath) }, mInfo { other.mInfo }, mImage { other.mImage }, mView { other.mView }, mMemoryInfo { other.mMemoryInfo }
	{
		other.mImage = VK_NULL_HANDLE;
		other.mView = VK_NULL_HANDLE;
		other.mMemoryInfo.mMemoryChunk = nullptr;
	}

	Image::~Image()
	{
		Service::get<Renderer>()->wait();
		const Device* kDevice { Service::get<Device>() };

		if (mView)
			vkDestroyImageView(kDevice->device(), mView, nullptr);

		if (mImage && mMemoryInfo.mMemoryChunk)
			vkDestroyImage(kDevice->device(), mImage, nullptr);

		if (mMemoryInfo.mMemoryChunk)
			Service::get<MemoryAllocator>()->free(mMemoryInfo);
	}
	
	void Image::initialize() 
	{
		if (mOnChange.empty())
		{
			mOnChange += [this]() -> void
			{
				Service::get<Renderer>()->wait();
				const Device* kDevice{ Service::get<Device>() };

				if (mView)
					vkDestroyImageView(kDevice->device(), mView, nullptr);

				if (mImage && mMemoryInfo.mMemoryChunk)
					vkDestroyImage(kDevice->device(), mImage, nullptr);

				if (mMemoryInfo.mMemoryChunk)
					Service::get<MemoryAllocator>()->free(mMemoryInfo);

				mInfo.mLayout = EImageLayout::UNDEFINED;
				initialize();
			};
		}

		Buffer::SubBuffer* staging { nullptr };
		math::Vec2ui extent { 0u, 0u };

		try
		{
			if (std::count(mkPath.begin(), mkPath.end(), ';') == 0u)
			{
				// Load single path
				std::vector<uint8_t> pixels {};
				Service::get<LoaderImg>()->load(mkPath.c_str(), pixels, extent, mInfo.format());

				size_t imageSize { pixels.size() };
				staging = Buffer::request(Buffer::EType::STAGING, static_cast<uint64_t>(imageSize));
				{
					Buffer::MemoryMap memMap { *staging };
					memcpy(memMap.mData, pixels.data(), imageSize);
				}
			}
			else
			{
				// Load cubemap paths
				auto split { [](std::string s, char delim, std::vector<std::string>& out) -> void
				{
					std::stringstream ss { s };
					std::string item {};

					while (std::getline(ss, item, delim))
						out.emplace_back(item);
				}
				};

				std::vector<std::string> paths;
				split(mkPath, ';', paths);

				std::vector<uint8_t> pixels[6u] {};
				for (uint8_t i { 0u }; i < 6u; ++i)
					Service::get<LoaderImg>()->load(paths[i].c_str(), pixels[i], extent, mInfo.format());

				size_t imageSize { pixels[0u].size() };
				staging = Buffer::request(Buffer::EType::STAGING, static_cast<uint64_t>(imageSize * 6u));
				{
					Buffer::MemoryMap memMap { *staging };
					for (uint8_t i { 0u }; i < 6u; ++i)
						std::memcpy(memMap.mData + imageSize * i, pixels[i].data(), imageSize);
				}
			}

			mInfo.mExtent = { extent.mX, extent.mY, 1u };
			mInfo.mUsage = EImageUsageFlag::TRANSFER_DST | EImageUsageFlag::SAMPLED | EImageUsageFlag::COLOR;

			createImage();
			createView();

			transfer(*staging);
		}
		catch (const std::exception& e)
		{
			Buffer::release(staging);
			THROW("failed to create image: " + std::string { e.what() })
		}

		Buffer::release(staging);
	}

	void Image::transitionLayout(EImageLayout newLayout)
	{
		Renderer::SingleTimeCommand commandBuffer {};
		recordLayoutTransition(commandBuffer.mBuffer, newLayout);
	}

	const Image::ImgInfo& Image::info() const
	{
		return mInfo;
	}

	VkImageView Image::view() const
	{
		return mView;
	}
	
	void Image::initialize(const uint8_t* kData, const math::Vec2ui& kExtent, const Format& kFormat)
	{
		Buffer::SubBuffer* staging { nullptr };

		mInfo.mType = kExtent.mY > 1u ? EImageType::SINGLE_2D : EImageType::SINGLE_1D;
		mInfo.mFormatComponent = kFormat.mComponent;
		mInfo.mFormatType = kFormat.mType;
		mInfo.mExtent = { kExtent.mX, kExtent.mY, 1u };
		mInfo.mUsage = EImageUsageFlag::TRANSFER_DST | EImageUsageFlag::SAMPLED | EImageUsageFlag::COLOR;

		try
		{
			uint32_t imageSize { kExtent.mX * kExtent.mY * toFormatComponentNumber(kFormat) };
			staging = Buffer::request(Buffer::EType::STAGING, imageSize);
			{
				Buffer::MemoryMap memMap { *staging };
				memcpy(memMap.mData, kData, imageSize);
			}

			createImage();
			createView();

			transfer(*staging);
		}
		catch (const std::exception& e)
		{
			Buffer::release(staging);
			THROW("failed to create image: " + std::string { e.what() })
		}

		Buffer::release(staging);
	}

	void Image::createImage()
	{
		const Device* kDevice { Service::get<Device>() };

		VkImageCreateInfo imageInfo { mInfo.imageInfo() };
		VkResult result { vkCreateImage(kDevice->device(), &imageInfo, nullptr, &mImage) };
		if (result != VK_SUCCESS)
			THROW("failed to create image with error: " + std::to_string(result))

		VkMemoryRequirements memRequirements {};
		vkGetImageMemoryRequirements(kDevice->device(), mImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = kDevice->findMemoryType(memRequirements.memoryTypeBits, mInfo.mMemory);

		mMemoryInfo = Service::get<MemoryAllocator>()->allocate(allocInfo);

		result = vkBindImageMemory(kDevice->device(), mImage, mMemoryInfo.mMemoryChunk->memory(), mMemoryInfo.mMemoryBlock->memoryOffset());
		if (result != VK_SUCCESS)
			THROW("failed to bind image memory with error: " + std::to_string(result))
	}

	void Image::createView()
	{
		if ((mInfo.mUsage & EImageUsageFlag::ATTACHMENT) != EImageUsageFlag::ATTACHMENT && (mInfo.mUsage & EImageUsageFlag::SAMPLED) != EImageUsageFlag::SAMPLED)
			return;

		VkImageViewCreateInfo viewInfo { mInfo.viewInfo(mImage) };
		VkResult result { vkCreateImageView(Service::get<Device>()->device(), &viewInfo, nullptr, &mView) };
		if (result != VK_SUCCESS)
			THROW("failed to create image view with error: " + std::to_string(result))
	}

	Image::PipelineBarrier Image::pipelineBarrier(VkImageLayout newLayout) const
	{
		VkImageMemoryBarrier barrier { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		barrier.oldLayout = toImageLayout(mInfo.mLayout);
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = mImage;
		barrier.subresourceRange.baseMipLevel = 0u;
		barrier.subresourceRange.levelCount = mInfo.mMipLevels;
		barrier.subresourceRange.baseArrayLayer = 0u;
		barrier.subresourceRange.layerCount = mInfo.mArrayLayers;
		barrier.subresourceRange.aspectMask = toImageAspect(mInfo.mUsage);

		// Stages & Access Mask
		VkPipelineStageFlags srcStage {}, dstStage {};

		// UNDEFINED -> TRANSFER_SRC
		if (barrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && barrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
		{
			barrier.srcAccessMask = 0u;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		// UNDEFINED -> TRANSFER_DST
		else if (barrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && barrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0u;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		// TRANSFER_DST -> GENERAL
		else if (barrier.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && barrier.newLayout == VK_IMAGE_LAYOUT_GENERAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		// GENERAL -> TRANSFER_DST
		else if (barrier.oldLayout == VK_IMAGE_LAYOUT_GENERAL && barrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		// TRANSFER_DST -> SHADER_READ_ONLY
		else if (barrier.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && barrier.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		// UNDEFINED -> DEPTH_STENCIL_ATTACHMENT
		else if (barrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && barrier.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0u;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		// UNDEFINED -> SHADER_READ_ONLY
		else if (barrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && barrier.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = 0u;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
			THROW("failed to transition image layout: transition not supported (" + std::to_string(barrier.oldLayout) + " to " + std::to_string(barrier.newLayout) + ")")

		return { barrier, srcStage, dstStage };
	}

	void Image::recordLayoutTransition(VkCommandBuffer commandBuffer, EImageLayout newLayout)
	{
		if (mInfo.mLayout == newLayout)
			return;

		PipelineBarrier barrier { pipelineBarrier(toImageLayout(newLayout)) };

		vkCmdPipelineBarrier(commandBuffer, barrier.mSrcFlags, barrier.mDstFlags, 0u, 0u, nullptr, 0u, nullptr, 1u, &barrier.mBarrier);
		mInfo.mLayout = newLayout;
	}

	void Image::transfer(Buffer::SubBuffer& buffer)
	{
		Renderer::SingleTimeCommand commandBuffer {};
		recordLayoutTransition(commandBuffer.mBuffer, EImageLayout::TRANSFER_DST);

		VkBufferImageCopy region {};

		region.bufferOffset = buffer.offset();
		region.imageSubresource.aspectMask = toImageAspect(mInfo.mUsage);
		region.imageSubresource.mipLevel = 0u;
		region.imageSubresource.baseArrayLayer = 0u;
		region.imageSubresource.layerCount = mInfo.mArrayLayers;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { mInfo.mExtent.mX, mInfo.mExtent.mY, mInfo.mExtent.mZ };

		vkCmdCopyBufferToImage(commandBuffer.mBuffer, buffer.buffer()->buffer(), mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		recordLayoutTransition(commandBuffer.mBuffer, EImageLayout::SHADER_READ_ONLY);
	}
}