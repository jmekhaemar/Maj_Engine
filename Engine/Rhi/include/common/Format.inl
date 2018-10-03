#ifndef _FORMAT_INL_
#define _FORMAT_INL_

#include "common/Format.h"
#include <Logging.h>

namespace maj::rhi
{
	DEFINE_FLAG(EFormatFeatureFlag, uint32_t)

	constexpr uint64_t toFormatSize(const Format& kFormat) noexcept
	{
		switch (kFormat.mComponent)
		{
			case Format::EComponent::S8:
			case Format::EComponent::R8:
			case Format::EComponent::R4G4:
			return 8u;

			case Format::EComponent::D16:
			case Format::EComponent::R16:
			case Format::EComponent::R8G8:
			case Format::EComponent::R5G6B5:
			case Format::EComponent::B5G6R5:
			case Format::EComponent::R5G5B5A1:
			case Format::EComponent::B5G5R5A1:
			case Format::EComponent::A1R5G5B5:
			case Format::EComponent::R4G4B4A4:
			case Format::EComponent::B4G4R4A4:
			return 16u;

			case Format::EComponent::D16S8:
			case Format::EComponent::R8G8B8:
			case Format::EComponent::B8G8R8:
			return 24u;

			case Format::EComponent::D32:
			case Format::EComponent::D24X8:
			case Format::EComponent::D24S8:
			case Format::EComponent::R32:
			case Format::EComponent::R16G16:
			case Format::EComponent::B10G11R11:
			case Format::EComponent::R8G8B8A8:
			case Format::EComponent::B8G8R8A8:
			case Format::EComponent::A8B8G8R8:
			case Format::EComponent::E5B9G9R9:
			case Format::EComponent::A2R10G10B10:
			case Format::EComponent::A2B10G10R10:
			return 32u;

			case Format::EComponent::D32S8:
			return 40u;

			case Format::EComponent::R16G16B16:
			return 48u;

			case Format::EComponent::R64:
			case Format::EComponent::R32G32:
			case Format::EComponent::R16G16B16A16:
			return 64u;

			case Format::EComponent::R32G32B32:
			return 96u;

			case Format::EComponent::R64G64:
			case Format::EComponent::R32G32B32A32:
			return 128u;

			case Format::EComponent::R64G64B64:
			return 192u;

			case Format::EComponent::R64G64B64A64:
			return 256u;

			case Format::EComponent::UNDEFINED:
			return 0u;

			default:
			LOG(LogWarning, "undefined format")
			return 0u;
		}
	}

	constexpr uint8_t toFormatComponentNumber(const Format& kFormat) noexcept
	{
		switch (kFormat.mComponent)
		{
			case Format::EComponent::S8:
			case Format::EComponent::R8:
			case Format::EComponent::D16:
			case Format::EComponent::R16:
			case Format::EComponent::D32:
			case Format::EComponent::D24X8:
			case Format::EComponent::R32:
			case Format::EComponent::R64:
			return 1u;

			case Format::EComponent::R4G4:
			case Format::EComponent::R8G8:
			case Format::EComponent::D16S8:
			case Format::EComponent::D24S8:
			case Format::EComponent::R16G16:
			case Format::EComponent::D32S8:
			case Format::EComponent::R32G32:
			case Format::EComponent::R64G64:
			return 2u;

			case Format::EComponent::R5G6B5:
			case Format::EComponent::B5G6R5:
			case Format::EComponent::R8G8B8:
			case Format::EComponent::B8G8R8:
			case Format::EComponent::B10G11R11:
			case Format::EComponent::R16G16B16:
			case Format::EComponent::R32G32B32:
			case Format::EComponent::R64G64B64:
			return 3u;

			case Format::EComponent::R5G5B5A1:
			case Format::EComponent::B5G5R5A1:
			case Format::EComponent::A1R5G5B5:
			case Format::EComponent::R4G4B4A4:
			case Format::EComponent::B4G4R4A4:
			case Format::EComponent::R8G8B8A8:
			case Format::EComponent::B8G8R8A8:
			case Format::EComponent::A8B8G8R8:
			case Format::EComponent::E5B9G9R9:
			case Format::EComponent::A2R10G10B10:
			case Format::EComponent::A2B10G10R10:
			case Format::EComponent::R16G16B16A16:
			case Format::EComponent::R32G32B32A32:
			case Format::EComponent::R64G64B64A64:
			return 4u;

			case Format::EComponent::UNDEFINED:
			return 0u;

			default:
			LOG(LogWarning, "undefined format")
			return 0u;
		}
	}
}

#endif