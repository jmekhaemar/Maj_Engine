#ifndef _RHI_IMAGE_H_
#define _RHI_IMAGE_H_

#include <Flag.h>
#include <MetaMacros.h>

namespace maj::rhi
{
	enum class EImageType
	{
		SINGLE_1D,
		SINGLE_2D,
		SINGLE_3D,
		CUBE,
		ARRAY_1D,
		ARRAY_2D,
		CUBE_ARRAY,
	};

	enum class EImageTiling
	{
		OPTIMAL,
		LINEAR
	};

	enum class ESampleFlag : uint8_t
	{
		COUNT_1 = 1u,
		COUNT_2 = 2u,
		COUNT_4 = 4u,
		COUNT_8 = 8u,
		COUNT_16 = 16u,
		COUNT_32 = 32u,
		COUNT_64 = 64u
	};

	enum class EImageLayout
	{
		UNDEFINED,
		PREINIT,
		COLOR,
		DEPTH_STENCIL_ATTACHMENT,
		DEPTH_STENCIL_READ_ONLY,
		SHADER_READ_ONLY,
		TRANSFER_SRC,
		TRANSFER_DST,
		PRESENT,
		SHARED_PRESENT,
		DEPTH_READ_ONLY_STENCIL_ATTACHMENT,
		DEPTH_ATTACHMENT_STENCIL_READ_ONLY,
		GENERAL
	};

	enum class EImageUsageFlag : uint16_t
	{
		NONE = 0u,
		SAMPLED = 1u,
		ATTACHMENT = 2u,
		COLOR = 4u,
		DEPTH = 8u,
		STENCIL = 16u,
		INPUT = 32u,
		STORAGE = 64u,
		TRANSFER_SRC = 128u,
		TRANSFER_DST = 256u
	};

	DECLARE_FLAG(ESampleFlag)
	DECLARE_FLAG(EImageUsageFlag)
}

META_ENUM(maj::rhi::EImageType, 7u)
META_ENUM_VALUE(maj::rhi::EImageType, maj::rhi::EImageType::SINGLE_1D)
META_ENUM_VALUE(maj::rhi::EImageType, maj::rhi::EImageType::SINGLE_2D)
META_ENUM_VALUE(maj::rhi::EImageType, maj::rhi::EImageType::SINGLE_3D)
META_ENUM_VALUE(maj::rhi::EImageType, maj::rhi::EImageType::CUBE)
META_ENUM_VALUE(maj::rhi::EImageType, maj::rhi::EImageType::ARRAY_1D)
META_ENUM_VALUE(maj::rhi::EImageType, maj::rhi::EImageType::ARRAY_2D)
META_ENUM_VALUE(maj::rhi::EImageType, maj::rhi::EImageType::CUBE_ARRAY)

META_ENUM(maj::rhi::EImageTiling, 2u)
META_ENUM_VALUE(maj::rhi::EImageTiling, maj::rhi::EImageTiling::OPTIMAL)
META_ENUM_VALUE(maj::rhi::EImageTiling, maj::rhi::EImageTiling::LINEAR)

META_FLAG(maj::rhi::ESampleFlag, 7u)
META_FLAG_VALUE(maj::rhi::ESampleFlag, maj::rhi::ESampleFlag::COUNT_1, 0u)
META_FLAG_VALUE(maj::rhi::ESampleFlag, maj::rhi::ESampleFlag::COUNT_2, 1u)
META_FLAG_VALUE(maj::rhi::ESampleFlag, maj::rhi::ESampleFlag::COUNT_4, 2u)
META_FLAG_VALUE(maj::rhi::ESampleFlag, maj::rhi::ESampleFlag::COUNT_8, 3u)
META_FLAG_VALUE(maj::rhi::ESampleFlag, maj::rhi::ESampleFlag::COUNT_16, 4u)
META_FLAG_VALUE(maj::rhi::ESampleFlag, maj::rhi::ESampleFlag::COUNT_32, 5u)
META_FLAG_VALUE(maj::rhi::ESampleFlag, maj::rhi::ESampleFlag::COUNT_64, 6u)

META_ENUM(maj::rhi::EImageLayout, 13u)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::UNDEFINED)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::PREINIT)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::COLOR)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::DEPTH_STENCIL_ATTACHMENT)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::DEPTH_STENCIL_READ_ONLY)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::SHADER_READ_ONLY)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::TRANSFER_SRC)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::TRANSFER_DST)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::PRESENT)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::SHARED_PRESENT)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY)
META_ENUM_VALUE(maj::rhi::EImageLayout, maj::rhi::EImageLayout::GENERAL)

META_FLAG(maj::rhi::EImageUsageFlag, 9u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::SAMPLED, 0u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::ATTACHMENT, 1u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::COLOR, 2u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::DEPTH, 3u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::STENCIL, 4u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::INPUT, 5u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::STORAGE, 6u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::TRANSFER_SRC, 7u)
META_FLAG_VALUE(maj::rhi::EImageUsageFlag, maj::rhi::EImageUsageFlag::TRANSFER_DST, 8u)

#include "common/Image.inl"

#endif