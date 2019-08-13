#ifndef _VK_IMAGE_H_
#define _VK_IMAGE_H_

#include <vector>
#include <vulkan/vulkan.h>

#include <NonCopyable.h>
#include <Vec3.h>
#include <Vec2.h>
#include <Asset.h>

#include "common/Format.h"
#include "common/Image.h"
#include "common/Memory.h"

#include "Buffer.h"
#include "MemoryAllocator.h"

namespace maj::rhi
{
	class Image final : public NonCopyable, public ISerializable
	{
		ENABLE_META(Image)

	public:
		struct ImgInfo final : public ISerializable
		{
			ENABLE_META(ImgInfo)

		public:
			LOAD_CONSTRUCTOR(ImgInfo)

			ImgInfo();
			~ImgInfo() = default;

			VkImageCreateInfo imageInfo() const;
			VkImageViewCreateInfo viewInfo(VkImage image) const;
			Format format() const;

			EImageType mType;
			Format::EComponent mFormatComponent;
			Format::EType mFormatType;
			math::Vec3ui mExtent;
			uint32_t mMipLevels;
			uint32_t mArrayLayers;
			ESampleFlag mSamples;
			EImageTiling mTiling;
			EImageUsageFlag mUsage;
			std::vector<uint32_t> mQueueFamilies;
			EImageLayout mLayout;
			EMemoryFlag mMemory;
		};

		struct MemoryMap final : public NonCopyable
		{
		public:
			MemoryMap(const Image& kImage);
			~MemoryMap();

			char* mData;
			uint32_t mRowStride;

		private:
			MemoryInfo mMemoryInfo;
		};

	private:
		struct PipelineBarrier final
		{
			PipelineBarrier(const VkImageMemoryBarrier& kBarrier);

			VkImageMemoryBarrier mBarrier;
			VkPipelineStageFlags mSrcFlags;
			VkPipelineStageFlags mDstFlags;
		};

		struct RegionInfo final // TODO move to helper 
		{
			EImageUsageFlag mUsage;
			uint32_t mMipLevel;
			uint32_t mBaseLayer;
			uint32_t mArrayLayers;
			math::Vec3i mOffset;
			math::Vec3i mExtent;
		};

	public:
		static void transitionLayout(std::vector<Image>& images, EImageLayout newLayout);
		static void transitionLayout(std::vector<Image>& images, const std::vector<EImageLayout>& kNewLayouts);
		static void copy(const Image& kSrc, Image& dst, EImageLayout dstLayout);

		static VkImageBlit toImageBlitInfo(const RegionInfo& kSrc, const RegionInfo& kDst); // TODO move to helper and constexpr

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Image)

		Image(const char* kPath, const Format& kFormat, EImageType type = EImageType::SINGLE_2D, EMemoryFlag properties = EMemoryFlag::DEVICE_LOCAL);
		Image(const char* kPath[6u], const Format& kFormat, EMemoryFlag properties = EMemoryFlag::DEVICE_LOCAL);
		Image(const uint8_t* kImage, const math::Vec2ui& kExtent, const Format& kFormat, EMemoryFlag properties = EMemoryFlag::DEVICE_LOCAL);
		Image(const ImgInfo& kInfo);
		Image(const ImgInfo& kInfo, VkImage image);
		Image(Image&& other);
		~Image();

		void transitionLayout(EImageLayout newLayout);

		const ImgInfo& info() const;
		VkImageView view() const;

		Event<void> mOnChange;

	private:
		void initialize(const uint8_t* kData, const math::Vec2ui& kExtent, const Format& kFormat);

		void createImage();
		void createView();
		void createMipmaps();

		PipelineBarrier pipelineBarrier(VkImageLayout newLayout) const;
		PipelineBarrier mipmapPipelineBarrier(VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipmapLevel) const;
		
		void recordLayoutTransition(VkCommandBuffer commandBuffer, EImageLayout newLayout);
		void transfer(Buffer::SubBuffer& buffer);

		const std::string mkPath;
		ImgInfo mInfo;

		VkImage mImage;
		VkImageView mView;
		MemoryInfo mMemoryInfo;
	};
}

META_CLASS(maj::Asset<maj::rhi::Image>, 1u)
META_FIELD(maj::Asset<maj::rhi::Image>, uint32_t, mGuid, 0u)

META_CLASS(maj::rhi::Image::ImgInfo, 9u)
META_FIELD(maj::rhi::Image::ImgInfo, maj::rhi::EImageType, mType, 0u)
META_FIELD(maj::rhi::Image::ImgInfo, maj::rhi::Format::EComponent, mFormatComponent, 1u)
META_FIELD(maj::rhi::Image::ImgInfo, maj::rhi::Format::EType, mFormatType, 2u)
META_FIELD(maj::rhi::Image::ImgInfo, uint32_t, mMipLevels, 3u)
META_FIELD(maj::rhi::Image::ImgInfo, uint32_t, mArrayLayers, 4u)
META_FIELD(maj::rhi::Image::ImgInfo, maj::rhi::ESampleFlag, mSamples, 5u)
META_FIELD(maj::rhi::Image::ImgInfo, maj::rhi::EImageTiling, mTiling, 6u)
META_FIELD(maj::rhi::Image::ImgInfo, maj::rhi::EImageUsageFlag, mUsage, 7u)
META_FIELD(maj::rhi::Image::ImgInfo, maj::rhi::EMemoryFlag, mMemory, 8u)

META_CLASS(maj::rhi::Image, 3u)
META_FIELD(maj::rhi::Image, uint32_t, mGuid, 0u)
META_FIELD(maj::rhi::Image, const std::string, mkPath, 1u)
META_FIELD(maj::rhi::Image, maj::rhi::Image::ImgInfo, mInfo, 2u)

#endif