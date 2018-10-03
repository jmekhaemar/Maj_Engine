#ifndef _VK_HELPER_INL_
#define _VK_HELPER_INL_

#include "VkHelper.h"

#include "common/Logging.h"

namespace maj::rhi
{
	constexpr VkFormat toFormat(const Format& kFormat) noexcept
	{
		switch (kFormat.mComponent)
		{
			case Format::EComponent::S8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_S8_UINT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R8_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R8_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R8_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R8_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R8_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R8_SINT;

					case Format::EType::SRGB:
					return VK_FORMAT_R8_SRGB;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R4G4:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R4G4_UNORM_PACK8;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::D16:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_D16_UNORM;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R16:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R16_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R16_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R16_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R16_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R16_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R16_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R16_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R8G8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R8G8_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R8G8_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R8G8_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R8G8_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R8G8_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R8G8_SINT;

					case Format::EType::SRGB:
					return VK_FORMAT_R8G8_SRGB;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R5G6B5:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R5G6B5_UNORM_PACK16;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::B5G6R5:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_B5G6R5_UNORM_PACK16;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R5G5B5A1:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R5G5B5A1_UNORM_PACK16;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::B5G5R5A1:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_B5G5R5A1_UNORM_PACK16;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::A1R5G5B5:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_A1R5G5B5_UNORM_PACK16;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R4G4B4A4:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R4G4B4A4_UNORM_PACK16;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::B4G4R4A4:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_B4G4R4A4_UNORM_PACK16;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::D16S8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					case Format::EType::UINT:
					return VK_FORMAT_D16_UNORM_S8_UINT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R8G8B8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R8G8B8_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R8G8B8_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R8G8B8_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R8G8B8_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R8G8B8_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R8G8B8_SINT;

					case Format::EType::SRGB:
					return VK_FORMAT_R8G8B8_SRGB;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::B8G8R8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_B8G8R8_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_B8G8R8_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_B8G8R8_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_B8G8R8_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_B8G8R8_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_B8G8R8_SINT;

					case Format::EType::SRGB:
					return VK_FORMAT_B8G8R8_SRGB;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::D32:
			{
				switch (kFormat.mType)
				{
					case Format::EType::SFLOAT:
					return VK_FORMAT_D32_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::D24X8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_X8_D24_UNORM_PACK32;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::D24S8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					case Format::EType::UINT:
					return VK_FORMAT_D24_UNORM_S8_UINT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R32:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R32_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R32_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R32_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R16G16:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R16G16_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R16G16_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R16G16_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R16G16_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R16G16_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R16G16_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R16G16_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::B10G11R11:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UFLOAT:
					return VK_FORMAT_B10G11R11_UFLOAT_PACK32;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R8G8B8A8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R8G8B8A8_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R8G8B8A8_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R8G8B8A8_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R8G8B8A8_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R8G8B8A8_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R8G8B8A8_SINT;

					case Format::EType::SRGB:
					return VK_FORMAT_R8G8B8A8_SRGB;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::B8G8R8A8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_B8G8R8A8_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_B8G8R8A8_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_B8G8R8A8_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_B8G8R8A8_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_B8G8R8A8_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_B8G8R8A8_SINT;

					case Format::EType::SRGB:
					return VK_FORMAT_B8G8R8A8_SRGB;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::A8B8G8R8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_A8B8G8R8_UNORM_PACK32;

					case Format::EType::SNORM:
					return VK_FORMAT_A8B8G8R8_SNORM_PACK32;

					case Format::EType::USCALED:
					return VK_FORMAT_A8B8G8R8_USCALED_PACK32;

					case Format::EType::SSCALED:
					return VK_FORMAT_A8B8G8R8_SSCALED_PACK32;

					case Format::EType::UINT:
					return VK_FORMAT_A8B8G8R8_UINT_PACK32;

					case Format::EType::SINT:
					return VK_FORMAT_A8B8G8R8_SINT_PACK32;

					case Format::EType::SRGB:
					return VK_FORMAT_A8B8G8R8_SRGB_PACK32;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::E5B9G9R9:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UFLOAT:
					return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::A2R10G10B10:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_A2R10G10B10_UNORM_PACK32;

					case Format::EType::SNORM:
					return VK_FORMAT_A2R10G10B10_SNORM_PACK32;

					case Format::EType::USCALED:
					return VK_FORMAT_A2R10G10B10_USCALED_PACK32;

					case Format::EType::SSCALED:
					return VK_FORMAT_A2R10G10B10_SSCALED_PACK32;

					case Format::EType::UINT:
					return VK_FORMAT_A2R10G10B10_UINT_PACK32;

					case Format::EType::SINT:
					return VK_FORMAT_A2R10G10B10_SINT_PACK32;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::A2B10G10R10:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_A2B10G10R10_UNORM_PACK32;

					case Format::EType::SNORM:
					return VK_FORMAT_A2B10G10R10_SNORM_PACK32;

					case Format::EType::USCALED:
					return VK_FORMAT_A2B10G10R10_USCALED_PACK32;

					case Format::EType::SSCALED:
					return VK_FORMAT_A2B10G10R10_SSCALED_PACK32;

					case Format::EType::UINT:
					return VK_FORMAT_A2B10G10R10_UINT_PACK32;

					case Format::EType::SINT:
					return VK_FORMAT_A2B10G10R10_SINT_PACK32;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::D32S8:
			{
				switch (kFormat.mType)
				{
					case Format::EType::SFLOAT:
					case Format::EType::UINT:
					return VK_FORMAT_D32_SFLOAT_S8_UINT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}
			
			case Format::EComponent::R16G16B16:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R16G16B16_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R16G16B16_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R16G16B16_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R16G16B16_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R16G16B16_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R16G16B16_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R16G16B16_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R64:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R64_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R64_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R64_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R32G32:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R32G32_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R32G32_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R32G32_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}
			
			case Format::EComponent::R16G16B16A16:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UNORM:
					return VK_FORMAT_R16G16B16A16_UNORM;

					case Format::EType::SNORM:
					return VK_FORMAT_R16G16B16A16_SNORM;

					case Format::EType::USCALED:
					return VK_FORMAT_R16G16B16A16_USCALED;

					case Format::EType::SSCALED:
					return VK_FORMAT_R16G16B16A16_SSCALED;

					case Format::EType::UINT:
					return VK_FORMAT_R16G16B16A16_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R16G16B16A16_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R16G16B16A16_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R32G32B32:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R32G32B32_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R32G32B32_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R32G32B32_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R64G64:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R64G64_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R64G64_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R64G64_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}
			
			case Format::EComponent::R32G32B32A32:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R32G32B32A32_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R32G32B32A32_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R32G32B32A32_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R64G64B64:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R64G64B64_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R64G64B64_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R64G64B64_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::R64G64B64A64:
			{
				switch (kFormat.mType)
				{
					case Format::EType::UINT:
					return VK_FORMAT_R64G64B64A64_UINT;

					case Format::EType::SINT:
					return VK_FORMAT_R64G64B64A64_SINT;

					case Format::EType::SFLOAT:
					return VK_FORMAT_R64G64B64A64_SFLOAT;

					default:
					LOG(LogRHIWarning, "undefined format")
					return VK_FORMAT_UNDEFINED;
				}
			}

			case Format::EComponent::UNDEFINED:
			return VK_FORMAT_UNDEFINED;

			default:
			LOG(LogRHIWarning, "undefined format")
			return VK_FORMAT_UNDEFINED;
		}
	}
	
	constexpr Format toFormat(const VkFormat kFormat) noexcept
	{
		switch (kFormat)
		{
			case VK_FORMAT_S8_UINT:
			return { Format::EComponent::S8, Format::EType::UINT };

			case VK_FORMAT_R8_UNORM:
			return { Format::EComponent::R8, Format::EType::UNORM };

			case VK_FORMAT_R8_SNORM:
			return { Format::EComponent::R8, Format::EType::SNORM };

			case VK_FORMAT_R8_USCALED:
			return { Format::EComponent::R8, Format::EType::USCALED };

			case VK_FORMAT_R8_SSCALED:
			return { Format::EComponent::R8, Format::EType::SSCALED };

			case VK_FORMAT_R8_UINT:
			return { Format::EComponent::R8, Format::EType::UINT };

			case VK_FORMAT_R8_SINT:
			return { Format::EComponent::R8, Format::EType::SINT };

			case VK_FORMAT_R8_SRGB:
			return { Format::EComponent::R8, Format::EType::SRGB };

			case VK_FORMAT_R4G4_UNORM_PACK8:
			return { Format::EComponent::R4G4, Format::EType::UNORM };

			case VK_FORMAT_D16_UNORM:
			return { Format::EComponent::D16, Format::EType::UNORM };

			case VK_FORMAT_R16_UNORM:
			return { Format::EComponent::R16, Format::EType::UNORM };

			case VK_FORMAT_R16_SNORM:
			return { Format::EComponent::R16, Format::EType::SNORM };

			case VK_FORMAT_R16_USCALED:
			return { Format::EComponent::R16, Format::EType::USCALED };

			case VK_FORMAT_R16_SSCALED:
			return { Format::EComponent::R16, Format::EType::SSCALED };

			case VK_FORMAT_R16_UINT:
			return { Format::EComponent::R16, Format::EType::UINT };

			case VK_FORMAT_R16_SINT:
			return { Format::EComponent::R16, Format::EType::SINT };

			case VK_FORMAT_R16_SFLOAT:
			return { Format::EComponent::R16, Format::EType::SFLOAT };

			case VK_FORMAT_R8G8_UNORM:
			return { Format::EComponent::R8G8, Format::EType::UNORM };

			case VK_FORMAT_R8G8_SNORM:
			return { Format::EComponent::R8G8, Format::EType::SNORM };

			case VK_FORMAT_R8G8_USCALED:
			return { Format::EComponent::R8G8, Format::EType::USCALED };

			case VK_FORMAT_R8G8_SSCALED:
			return { Format::EComponent::R8G8, Format::EType::SSCALED };

			case VK_FORMAT_R8G8_UINT:
			return { Format::EComponent::R8G8, Format::EType::UINT };

			case VK_FORMAT_R8G8_SINT:
			return { Format::EComponent::R8G8, Format::EType::SINT };

			case VK_FORMAT_R8G8_SRGB:
			return { Format::EComponent::R8G8, Format::EType::SRGB };

			case VK_FORMAT_R5G6B5_UNORM_PACK16:
			return { Format::EComponent::R5G6B5, Format::EType::UNORM };

			case VK_FORMAT_B5G6R5_UNORM_PACK16:
			return { Format::EComponent::B5G6R5, Format::EType::UNORM };

			case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
			return { Format::EComponent::R5G5B5A1, Format::EType::UNORM };

			case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
			return { Format::EComponent::B5G5R5A1, Format::EType::UNORM };

			case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
			return { Format::EComponent::A1R5G5B5, Format::EType::UNORM };

			case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
			return { Format::EComponent::R4G4B4A4, Format::EType::UNORM };

			case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
			return { Format::EComponent::B4G4R4A4, Format::EType::UNORM };

			case VK_FORMAT_D16_UNORM_S8_UINT:
			return { Format::EComponent::D16S8, Format::EType::UINT };

			case VK_FORMAT_R8G8B8_UNORM:
			return { Format::EComponent::R8G8B8, Format::EType::UNORM };

			case VK_FORMAT_R8G8B8_SNORM:
			return { Format::EComponent::R8G8B8, Format::EType::SNORM };

			case VK_FORMAT_R8G8B8_USCALED:
			return { Format::EComponent::R8G8B8, Format::EType::USCALED };

			case VK_FORMAT_R8G8B8_SSCALED:
			return { Format::EComponent::R8G8B8, Format::EType::SSCALED };

			case VK_FORMAT_R8G8B8_UINT:
			return { Format::EComponent::R8G8B8, Format::EType::UINT };

			case VK_FORMAT_R8G8B8_SINT:
			return { Format::EComponent::R8G8B8, Format::EType::SINT };

			case VK_FORMAT_R8G8B8_SRGB:
			return { Format::EComponent::R8G8B8, Format::EType::SRGB };

			case VK_FORMAT_B8G8R8_UNORM:
			return { Format::EComponent::B8G8R8, Format::EType::UNORM };

			case VK_FORMAT_B8G8R8_SNORM:
			return { Format::EComponent::B8G8R8, Format::EType::SNORM };

			case VK_FORMAT_B8G8R8_USCALED:
			return { Format::EComponent::B8G8R8, Format::EType::USCALED };

			case VK_FORMAT_B8G8R8_SSCALED:
			return { Format::EComponent::B8G8R8, Format::EType::SSCALED };

			case VK_FORMAT_B8G8R8_UINT:
			return { Format::EComponent::B8G8R8, Format::EType::UINT };

			case VK_FORMAT_B8G8R8_SINT:
			return { Format::EComponent::B8G8R8, Format::EType::SINT };

			case VK_FORMAT_B8G8R8_SRGB:
			return { Format::EComponent::B8G8R8, Format::EType::SRGB };

			case VK_FORMAT_D32_SFLOAT:
			return { Format::EComponent::D32, Format::EType::SFLOAT };

			case VK_FORMAT_X8_D24_UNORM_PACK32:
			return { Format::EComponent::D24X8, Format::EType::UNORM };

			case VK_FORMAT_D24_UNORM_S8_UINT:
			return { Format::EComponent::D24S8, Format::EType::UNORM };

			case VK_FORMAT_R32_UINT:
			return { Format::EComponent::R32, Format::EType::UINT };

			case VK_FORMAT_R32_SINT:
			return { Format::EComponent::R32, Format::EType::SINT };

			case VK_FORMAT_R32_SFLOAT:
			return { Format::EComponent::R32, Format::EType::SFLOAT };

			case VK_FORMAT_R16G16_UNORM:
			return { Format::EComponent::R16G16, Format::EType::UNORM };

			case VK_FORMAT_R16G16_SNORM:
			return { Format::EComponent::R16G16, Format::EType::SNORM };

			case VK_FORMAT_R16G16_USCALED:
			return { Format::EComponent::R16G16, Format::EType::USCALED };

			case VK_FORMAT_R16G16_SSCALED:
			return { Format::EComponent::R16G16, Format::EType::SSCALED };

			case VK_FORMAT_R16G16_UINT:
			return { Format::EComponent::R16G16, Format::EType::UINT };

			case VK_FORMAT_R16G16_SINT:
			return { Format::EComponent::R16G16, Format::EType::SINT };

			case VK_FORMAT_R16G16_SFLOAT:
			return { Format::EComponent::R16G16, Format::EType::SFLOAT };

			case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
			return { Format::EComponent::B10G11R11, Format::EType::UFLOAT };

			case VK_FORMAT_R8G8B8A8_UNORM:
			return { Format::EComponent::R8G8B8A8, Format::EType::UNORM };

			case VK_FORMAT_R8G8B8A8_SNORM:
			return { Format::EComponent::R8G8B8A8, Format::EType::SNORM };

			case VK_FORMAT_R8G8B8A8_USCALED:
			return { Format::EComponent::R8G8B8A8, Format::EType::USCALED };

			case VK_FORMAT_R8G8B8A8_SSCALED:
			return { Format::EComponent::R8G8B8A8, Format::EType::SSCALED };

			case VK_FORMAT_R8G8B8A8_UINT:
			return { Format::EComponent::R8G8B8A8, Format::EType::UINT };

			case VK_FORMAT_R8G8B8A8_SINT:
			return { Format::EComponent::R8G8B8A8, Format::EType::SINT };

			case VK_FORMAT_R8G8B8A8_SRGB:
			return { Format::EComponent::R8G8B8A8, Format::EType::SRGB };

			case VK_FORMAT_B8G8R8A8_UNORM:
			return { Format::EComponent::B8G8R8A8, Format::EType::UNORM };

			case VK_FORMAT_B8G8R8A8_SNORM:
			return { Format::EComponent::B8G8R8A8, Format::EType::SNORM };

			case VK_FORMAT_B8G8R8A8_USCALED:
			return { Format::EComponent::B8G8R8A8, Format::EType::USCALED };

			case VK_FORMAT_B8G8R8A8_SSCALED:
			return { Format::EComponent::B8G8R8A8, Format::EType::SSCALED };

			case VK_FORMAT_B8G8R8A8_UINT:
			return { Format::EComponent::B8G8R8A8, Format::EType::UINT };

			case VK_FORMAT_B8G8R8A8_SINT:
			return { Format::EComponent::B8G8R8A8, Format::EType::SINT };

			case VK_FORMAT_B8G8R8A8_SRGB:
			return { Format::EComponent::B8G8R8A8, Format::EType::SRGB };

			case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
			return { Format::EComponent::A8B8G8R8, Format::EType::UNORM };

			case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
			return { Format::EComponent::A8B8G8R8, Format::EType::SNORM };

			case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
			return { Format::EComponent::A8B8G8R8, Format::EType::USCALED };

			case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
			return { Format::EComponent::A8B8G8R8, Format::EType::SSCALED };

			case VK_FORMAT_A8B8G8R8_UINT_PACK32:
			return { Format::EComponent::A8B8G8R8, Format::EType::UINT };

			case VK_FORMAT_A8B8G8R8_SINT_PACK32:
			return { Format::EComponent::A8B8G8R8, Format::EType::SINT };

			case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
			return { Format::EComponent::A8B8G8R8, Format::EType::SRGB };

			case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
			return { Format::EComponent::E5B9G9R9, Format::EType::UFLOAT };

			case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
			return { Format::EComponent::A2R10G10B10, Format::EType::UNORM };

			case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
			return { Format::EComponent::A2R10G10B10, Format::EType::SNORM };

			case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
			return { Format::EComponent::A2R10G10B10, Format::EType::USCALED };

			case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
			return { Format::EComponent::A2R10G10B10, Format::EType::SSCALED };

			case VK_FORMAT_A2R10G10B10_UINT_PACK32:
			return { Format::EComponent::A2R10G10B10, Format::EType::UINT };

			case VK_FORMAT_A2R10G10B10_SINT_PACK32:
			return { Format::EComponent::A2R10G10B10, Format::EType::SINT };

			case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
			return { Format::EComponent::A2B10G10R10, Format::EType::UNORM };

			case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
			return { Format::EComponent::A2B10G10R10, Format::EType::SNORM };

			case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
			return { Format::EComponent::A2B10G10R10, Format::EType::USCALED };

			case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
			return { Format::EComponent::A2B10G10R10, Format::EType::SSCALED };

			case VK_FORMAT_A2B10G10R10_UINT_PACK32:
			return { Format::EComponent::A2B10G10R10, Format::EType::UINT };

			case VK_FORMAT_A2B10G10R10_SINT_PACK32:
			return { Format::EComponent::A2B10G10R10, Format::EType::SINT };

			case VK_FORMAT_D32_SFLOAT_S8_UINT:
			return { Format::EComponent::D32S8, Format::EType::SFLOAT };

			case VK_FORMAT_R16G16B16_UNORM:
			return { Format::EComponent::R16G16B16, Format::EType::UNORM };

			case VK_FORMAT_R16G16B16_SNORM:
			return { Format::EComponent::R16G16B16, Format::EType::SNORM };

			case VK_FORMAT_R16G16B16_USCALED:
			return { Format::EComponent::R16G16B16, Format::EType::USCALED };

			case VK_FORMAT_R16G16B16_SSCALED:
			return { Format::EComponent::R16G16B16, Format::EType::SSCALED };

			case VK_FORMAT_R16G16B16_UINT:
			return { Format::EComponent::R16G16B16, Format::EType::UINT };

			case VK_FORMAT_R16G16B16_SINT:
			return { Format::EComponent::R16G16B16, Format::EType::SINT };

			case VK_FORMAT_R16G16B16_SFLOAT:
			return { Format::EComponent::R16G16B16, Format::EType::SFLOAT };

			case VK_FORMAT_R64_UINT:
			return { Format::EComponent::R64, Format::EType::UINT };

			case VK_FORMAT_R64_SINT:
			return { Format::EComponent::R64, Format::EType::SINT };

			case VK_FORMAT_R64_SFLOAT:
			return { Format::EComponent::R64, Format::EType::SFLOAT };

			case VK_FORMAT_R32G32_UINT:
			return { Format::EComponent::R32G32, Format::EType::UINT };

			case VK_FORMAT_R32G32_SINT:
			return { Format::EComponent::R32G32, Format::EType::SINT };

			case VK_FORMAT_R32G32_SFLOAT:
			return { Format::EComponent::R32G32, Format::EType::SFLOAT };

			case VK_FORMAT_R16G16B16A16_UNORM:
			return { Format::EComponent::R16G16B16A16, Format::EType::UNORM };

			case VK_FORMAT_R16G16B16A16_SNORM:
			return { Format::EComponent::R16G16B16A16, Format::EType::SNORM };

			case VK_FORMAT_R16G16B16A16_USCALED:
			return { Format::EComponent::R16G16B16A16, Format::EType::USCALED };

			case VK_FORMAT_R16G16B16A16_SSCALED:
			return { Format::EComponent::R16G16B16A16, Format::EType::SSCALED };

			case VK_FORMAT_R16G16B16A16_UINT:
			return { Format::EComponent::R16G16B16A16, Format::EType::UINT };

			case VK_FORMAT_R16G16B16A16_SINT:
			return { Format::EComponent::R16G16B16A16, Format::EType::SINT };

			case VK_FORMAT_R16G16B16A16_SFLOAT:
			return { Format::EComponent::R16G16B16A16, Format::EType::SFLOAT };

			case VK_FORMAT_R32G32B32_UINT:
			return { Format::EComponent::R32G32B32, Format::EType::UINT };

			case VK_FORMAT_R32G32B32_SINT:
			return { Format::EComponent::R32G32B32, Format::EType::SINT };

			case VK_FORMAT_R32G32B32_SFLOAT:
			return { Format::EComponent::R32G32B32, Format::EType::SFLOAT };

			case VK_FORMAT_R64G64_UINT:
			return { Format::EComponent::R64G64, Format::EType::UINT };

			case VK_FORMAT_R64G64_SINT:
			return { Format::EComponent::R64G64, Format::EType::SINT };

			case VK_FORMAT_R64G64_SFLOAT:
			return { Format::EComponent::R64G64, Format::EType::SFLOAT };

			case VK_FORMAT_R32G32B32A32_UINT:
			return { Format::EComponent::R32G32B32A32, Format::EType::UINT };

			case VK_FORMAT_R32G32B32A32_SINT:
			return { Format::EComponent::R32G32B32A32, Format::EType::SINT };

			case VK_FORMAT_R32G32B32A32_SFLOAT:
			return { Format::EComponent::R32G32B32A32, Format::EType::SFLOAT };

			case VK_FORMAT_R64G64B64_UINT:
			return { Format::EComponent::R64G64B64, Format::EType::UINT };

			case VK_FORMAT_R64G64B64_SINT:
			return { Format::EComponent::R64G64B64, Format::EType::SINT };

			case VK_FORMAT_R64G64B64_SFLOAT:
			return { Format::EComponent::R64G64B64, Format::EType::SFLOAT };

			case VK_FORMAT_R64G64B64A64_UINT:
			return { Format::EComponent::R64G64B64A64, Format::EType::UINT };

			case VK_FORMAT_R64G64B64A64_SINT:
			return { Format::EComponent::R64G64B64A64, Format::EType::SINT };

			case VK_FORMAT_R64G64B64A64_SFLOAT:
			return { Format::EComponent::R64G64B64A64, Format::EType::SFLOAT };
			
			case VK_FORMAT_UNDEFINED:
			return { Format::EComponent::UNDEFINED, Format::EType::UNORM };

			default:
			LOG(LogRHIWarning, "undefined format")
			return { Format::EComponent::UNDEFINED, Format::EType::UNORM };
		}
	}

	constexpr VkFormatFeatureFlagBits toFormatFeatureFlag(EFormatFeatureFlag feature) noexcept
	{
		int32_t result { 0 };

		if ((feature & EFormatFeatureFlag::SAMPLED_IMAGE) == EFormatFeatureFlag::SAMPLED_IMAGE)
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;

		if ((feature & EFormatFeatureFlag::UNIFORM_TEXEL_BUFFER) == EFormatFeatureFlag::UNIFORM_TEXEL_BUFFER)
			result |= VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT;

		if ((feature & EFormatFeatureFlag::STORAGE_IMAGE) == EFormatFeatureFlag::STORAGE_IMAGE)
			result |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;

		if ((feature & EFormatFeatureFlag::STORAGE_TEXEL_BUFFER) == EFormatFeatureFlag::STORAGE_TEXEL_BUFFER)
			result |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT;

		if ((feature & EFormatFeatureFlag::STORAGE_ATOMIC_IMAGE) == EFormatFeatureFlag::STORAGE_ATOMIC_IMAGE)
			result |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;

		if ((feature & EFormatFeatureFlag::STORAGE_ATOMIC_TEXEL_BUFFER) == EFormatFeatureFlag::STORAGE_ATOMIC_TEXEL_BUFFER)
			result |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT;

		if ((feature & EFormatFeatureFlag::VERTEX_BUFFER) == EFormatFeatureFlag::VERTEX_BUFFER)
			result |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;

		if ((feature & EFormatFeatureFlag::COLOR_ATTACHMENT) == EFormatFeatureFlag::COLOR_ATTACHMENT)
			result |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;

		if ((feature & EFormatFeatureFlag::COLOR_ATTACHMENT_BLEND) == EFormatFeatureFlag::COLOR_ATTACHMENT_BLEND)
			result |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;

		if ((feature & EFormatFeatureFlag::DEPTH_STENCIL_ATTACHMENT) == EFormatFeatureFlag::DEPTH_STENCIL_ATTACHMENT)
			result |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

		if ((feature & EFormatFeatureFlag::BLIT_SRC) == EFormatFeatureFlag::BLIT_SRC)
			result |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;

		if ((feature & EFormatFeatureFlag::BLIT_DST) == EFormatFeatureFlag::BLIT_DST)
			result |= VK_FORMAT_FEATURE_BLIT_DST_BIT;

		if ((feature & EFormatFeatureFlag::TRANSFER_SRC) == EFormatFeatureFlag::TRANSFER_SRC)
			result |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR;

		if ((feature & EFormatFeatureFlag::TRANSFER_DST) == EFormatFeatureFlag::TRANSFER_DST)
			result |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR;

		if ((feature & EFormatFeatureFlag::SAMPLED_FILTER_LINEAR) == EFormatFeatureFlag::SAMPLED_FILTER_LINEAR)
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;

		if ((feature & EFormatFeatureFlag::SAMPLED_FILTER_CUBIC) == EFormatFeatureFlag::SAMPLED_FILTER_CUBIC)
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG;

		if ((feature & EFormatFeatureFlag::SAMPLED_FILTER_MINMAX) == EFormatFeatureFlag::SAMPLED_FILTER_MINMAX)
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT;

		return static_cast<VkFormatFeatureFlagBits>(result);
	}

	constexpr VkColorSpaceKHR toColorSpace(EColorSpace colorSpace) noexcept
	{
		switch (colorSpace)
		{
			case EColorSpace::SRGB:
			return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

			default:
			LOG(LogRHIWarning, "undefined color space")
			return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		}
	}

	constexpr EColorSpace toColorSpace(VkColorSpaceKHR colorSpace) noexcept
	{
		switch (colorSpace)
		{
			case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
			return EColorSpace::SRGB;

			default:
			LOG(LogRHIWarning, "undefined color space")
			return EColorSpace::UNDEFINED;
		}
	}

	constexpr VkMemoryPropertyFlagBits toMemoryFlag(EMemoryFlag memory) noexcept
	{
		int32_t result { 0 };

		if ((memory & EMemoryFlag::DEVICE_LOCAL) == EMemoryFlag::DEVICE_LOCAL)
			result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		if ((memory & EMemoryFlag::HOST_VISIBLE) == EMemoryFlag::HOST_VISIBLE)
			result |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

		if ((memory & EMemoryFlag::HOST_COHERENT) == EMemoryFlag::HOST_COHERENT)
			result |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		if ((memory & EMemoryFlag::HOST_CACHED) == EMemoryFlag::HOST_CACHED)
			result |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

		if ((memory & EMemoryFlag::LAZY_ALLOC) == EMemoryFlag::LAZY_ALLOC)
			result |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;

		return static_cast<VkMemoryPropertyFlagBits>(result);
	}

	constexpr VkImageTiling toImageTiling(EImageTiling tiling) noexcept
	{
		switch (tiling)
		{
			case EImageTiling::OPTIMAL:
			return VK_IMAGE_TILING_OPTIMAL;

			case EImageTiling::LINEAR:
			return VK_IMAGE_TILING_LINEAR;
			
			default:
			return VK_IMAGE_TILING_OPTIMAL;
		}
	}

	constexpr VkImageType toImageType(EImageType type) noexcept
	{
		switch (type)
		{
			case EImageType::SINGLE_1D:
			case EImageType::ARRAY_1D:
			return VK_IMAGE_TYPE_1D;

			case EImageType::SINGLE_2D:
			case EImageType::CUBE:
			case EImageType::ARRAY_2D:
			case EImageType::CUBE_ARRAY:
			return VK_IMAGE_TYPE_2D;

			case EImageType::SINGLE_3D:
			return VK_IMAGE_TYPE_3D;

			default:
			return VK_IMAGE_TYPE_2D;
		}
	}

	constexpr VkImageViewType toImageViewType(EImageType type) noexcept
	{
		switch (type)
		{
			case EImageType::SINGLE_1D:
			return VK_IMAGE_VIEW_TYPE_1D;

			case EImageType::SINGLE_2D:
			return VK_IMAGE_VIEW_TYPE_2D;

			case EImageType::SINGLE_3D:
			return VK_IMAGE_VIEW_TYPE_3D;

			case EImageType::CUBE:
			return VK_IMAGE_VIEW_TYPE_CUBE;

			case EImageType::ARRAY_1D:
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

			case EImageType::ARRAY_2D:
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

			case EImageType::CUBE_ARRAY:
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

			default:
			return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	constexpr VkImageUsageFlagBits toImageUsage(EImageUsageFlag usage) noexcept
	{
		int32_t result { 0 };

		if ((usage & EImageUsageFlag::SAMPLED) == EImageUsageFlag::SAMPLED)
			result |= VK_IMAGE_USAGE_SAMPLED_BIT;

		if ((usage & EImageUsageFlag::ATTACHMENT) == EImageUsageFlag::ATTACHMENT)
		{
			if ((usage & EImageUsageFlag::COLOR) == EImageUsageFlag::COLOR)
				result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			if ((usage & EImageUsageFlag::DEPTH) == EImageUsageFlag::DEPTH || (usage & EImageUsageFlag::STENCIL) == EImageUsageFlag::STENCIL)
				result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			if ((usage & EImageUsageFlag::INPUT) == EImageUsageFlag::INPUT)
				result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		}

		if ((usage & EImageUsageFlag::STORAGE) == EImageUsageFlag::STORAGE)
			result |= VK_IMAGE_USAGE_STORAGE_BIT;

		if ((usage & EImageUsageFlag::TRANSFER_SRC) == EImageUsageFlag::TRANSFER_SRC)
			result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		if ((usage & EImageUsageFlag::TRANSFER_DST) == EImageUsageFlag::TRANSFER_DST)
			result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		return static_cast<VkImageUsageFlagBits>(result);
	}

	constexpr VkImageAspectFlagBits toImageAspect(EImageUsageFlag usage) noexcept
	{
		int32_t result { 0 };

		if ((usage & EImageUsageFlag::COLOR) == EImageUsageFlag::COLOR)
			result |= VK_IMAGE_ASPECT_COLOR_BIT;

		if ((usage & EImageUsageFlag::DEPTH) == EImageUsageFlag::DEPTH)
			result |= VK_IMAGE_ASPECT_DEPTH_BIT;

		if ((usage & EImageUsageFlag::STENCIL) == EImageUsageFlag::STENCIL)
			result |= VK_IMAGE_ASPECT_STENCIL_BIT;

		return static_cast<VkImageAspectFlagBits>(result);
	}

	constexpr VkSampleCountFlagBits toSampleFlag(ESampleFlag sample) noexcept
	{
		int32_t result { 0 };

		if ((sample & ESampleFlag::COUNT_1) == ESampleFlag::COUNT_1)
			result |= VK_SAMPLE_COUNT_1_BIT;

		if ((sample & ESampleFlag::COUNT_2) == ESampleFlag::COUNT_2)
			result |= VK_SAMPLE_COUNT_2_BIT;

		if ((sample & ESampleFlag::COUNT_4) == ESampleFlag::COUNT_4)
			result |= VK_SAMPLE_COUNT_4_BIT;

		if ((sample & ESampleFlag::COUNT_8) == ESampleFlag::COUNT_8)
			result |= VK_SAMPLE_COUNT_8_BIT;

		if ((sample & ESampleFlag::COUNT_16) == ESampleFlag::COUNT_16)
			result |= VK_SAMPLE_COUNT_16_BIT;

		if ((sample & ESampleFlag::COUNT_32) == ESampleFlag::COUNT_32)
			result |= VK_SAMPLE_COUNT_32_BIT;

		if ((sample & ESampleFlag::COUNT_64) == ESampleFlag::COUNT_64)
			result |= VK_SAMPLE_COUNT_64_BIT;

		return static_cast<VkSampleCountFlagBits>(result);
	}

	constexpr VkImageLayout toImageLayout(EImageLayout layout) noexcept
	{
		switch (layout)
		{
			case EImageLayout::PREINIT:
			return VK_IMAGE_LAYOUT_PREINITIALIZED;

			case EImageLayout::COLOR:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			case EImageLayout::DEPTH_STENCIL_ATTACHMENT:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			case EImageLayout::DEPTH_STENCIL_READ_ONLY:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

			case EImageLayout::SHADER_READ_ONLY:
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			case EImageLayout::TRANSFER_SRC:
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

			case EImageLayout::TRANSFER_DST:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

			case EImageLayout::PRESENT:
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			case EImageLayout::SHARED_PRESENT:
			return VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;

			case EImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT:
			return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR;

			case EImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY:
			return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR;

			case EImageLayout::GENERAL:
			return VK_IMAGE_LAYOUT_GENERAL;

			default:
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}

	constexpr VkPipelineStageFlagBits toPipelineStageFlag(EPipelineStageFlag stage) noexcept
	{
		int32_t result { 0 };

		if ((stage & EPipelineStageFlag::TOP) == EPipelineStageFlag::TOP)
			result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		if ((stage & EPipelineStageFlag::DRAW_INDIRECT) == EPipelineStageFlag::DRAW_INDIRECT)
			result |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;

		if ((stage & EPipelineStageFlag::VERTEX_INPUT) == EPipelineStageFlag::VERTEX_INPUT)
			result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

		if ((stage & EPipelineStageFlag::VERTEX_SHADER) == EPipelineStageFlag::VERTEX_SHADER)
			result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

		if ((stage & EPipelineStageFlag::TESS_CONTROL_SHADER) == EPipelineStageFlag::TESS_CONTROL_SHADER)
			result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;

		if ((stage & EPipelineStageFlag::TESS_EVAL_SHADER) == EPipelineStageFlag::TESS_EVAL_SHADER)
			result |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;

		if ((stage & EPipelineStageFlag::GEOMETRY_SHADER) == EPipelineStageFlag::GEOMETRY_SHADER)
			result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;

		if ((stage & EPipelineStageFlag::FRAGMENT_SHADER) == EPipelineStageFlag::FRAGMENT_SHADER)
			result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		if ((stage & EPipelineStageFlag::EARLY_FRAGMENT) == EPipelineStageFlag::EARLY_FRAGMENT)
			result |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

		if ((stage & EPipelineStageFlag::LATE_FRAGMENT) == EPipelineStageFlag::LATE_FRAGMENT)
			result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

		if ((stage & EPipelineStageFlag::COLOR_OUTPUT) == EPipelineStageFlag::COLOR_OUTPUT)
			result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		if ((stage & EPipelineStageFlag::COMPUTE_SHADER) == EPipelineStageFlag::COMPUTE_SHADER)
			result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

		if ((stage & EPipelineStageFlag::TRANSFER) == EPipelineStageFlag::TRANSFER)
			result |= VK_PIPELINE_STAGE_TRANSFER_BIT;

		if ((stage & EPipelineStageFlag::BOTTOM) == EPipelineStageFlag::BOTTOM)
			result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		if ((stage & EPipelineStageFlag::ALL_COMMANDS) == EPipelineStageFlag::ALL_COMMANDS)
			result |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

		return static_cast<VkPipelineStageFlagBits>(result);
	}

	constexpr VkAccessFlagBits toAccessFlag(EAccessFlag access) noexcept
	{
		int32_t result { 0 };

		if ((access & EAccessFlag::INDIRECT_READ) == EAccessFlag::INDIRECT_READ)
			result |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

		if ((access & EAccessFlag::INDEX_READ) == EAccessFlag::INDEX_READ)
			result |= VK_ACCESS_INDEX_READ_BIT;

		if ((access & EAccessFlag::VERTEX_ATTRIB_READ) == EAccessFlag::VERTEX_ATTRIB_READ)
			result |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

		if ((access & EAccessFlag::UNIFORM_READ) == EAccessFlag::UNIFORM_READ)
			result |= VK_ACCESS_UNIFORM_READ_BIT;

		if ((access & EAccessFlag::INPUT_READ) == EAccessFlag::INPUT_READ)
			result |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;

		if ((access & EAccessFlag::SHADER_READ) == EAccessFlag::SHADER_READ)
			result |= VK_ACCESS_SHADER_READ_BIT;

		if ((access & EAccessFlag::SHADER_WRITE) == EAccessFlag::SHADER_WRITE)
			result |= VK_ACCESS_SHADER_WRITE_BIT;

		if ((access & EAccessFlag::COLOR_READ) == EAccessFlag::COLOR_READ)
			result |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;

		if ((access & EAccessFlag::COLOR_WRITE) == EAccessFlag::COLOR_WRITE)
			result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		if ((access & EAccessFlag::DEPTH_STENCIL_READ) == EAccessFlag::DEPTH_STENCIL_READ)
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

		if ((access & EAccessFlag::DEPTH_STENCIL_WRITE) == EAccessFlag::DEPTH_STENCIL_WRITE)
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		if ((access & EAccessFlag::TRANSFER_READ) == EAccessFlag::TRANSFER_READ)
			result |= VK_ACCESS_TRANSFER_READ_BIT;

		if ((access & EAccessFlag::TRANSFER_WRITE) == EAccessFlag::TRANSFER_WRITE)
			result |= VK_ACCESS_TRANSFER_WRITE_BIT;

		if ((access & EAccessFlag::MEMORY_READ) == EAccessFlag::MEMORY_READ)
			result |= VK_ACCESS_MEMORY_READ_BIT;

		if ((access & EAccessFlag::MEMORY_WRITE) == EAccessFlag::MEMORY_WRITE)
			result |= VK_ACCESS_MEMORY_WRITE_BIT;

		return static_cast<VkAccessFlagBits>(result);
	}
	
	constexpr VkShaderStageFlagBits toShaderStageFlag(EShaderStageFlag stage) noexcept
	{
		int32_t result { 0 };

		if ((stage & EShaderStageFlag::VERTEX) == EShaderStageFlag::VERTEX)
			result |= VK_SHADER_STAGE_VERTEX_BIT;

		if ((stage & EShaderStageFlag::TESS_CONTROL) == EShaderStageFlag::TESS_CONTROL)
			result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

		if ((stage & EShaderStageFlag::TESS_EVAL) == EShaderStageFlag::TESS_EVAL)
			result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		if ((stage & EShaderStageFlag::GEOMETRY) == EShaderStageFlag::GEOMETRY)
			result |= VK_SHADER_STAGE_GEOMETRY_BIT;

		if ((stage & EShaderStageFlag::FRAGMENT) == EShaderStageFlag::FRAGMENT)
			result |= VK_SHADER_STAGE_FRAGMENT_BIT;

		if ((stage & EShaderStageFlag::COMPUTE) == EShaderStageFlag::COMPUTE)
			result |= VK_SHADER_STAGE_COMPUTE_BIT;

		if ((stage & EShaderStageFlag::ALL_GRAPHICS) == EShaderStageFlag::ALL_GRAPHICS)
			result |= VK_SHADER_STAGE_ALL_GRAPHICS;

		if ((stage & EShaderStageFlag::ALL) == EShaderStageFlag::ALL)
			result |= VK_SHADER_STAGE_ALL;

		return static_cast<VkShaderStageFlagBits>(result);
	}

	constexpr VkDescriptorType toDescriptorType(EDescriptorType type) noexcept
	{
		switch (type)
		{
			case EDescriptorType::SAMPLER:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

			case EDescriptorType::TEXTURE:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

			case EDescriptorType::IMAGE:
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

			case EDescriptorType::STORAGE_IMAGE:
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

			case EDescriptorType::UNIFORM_TEXEL_BUFFER:
			return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;

			case EDescriptorType::STORAGE_TEXEL_BUFFER:
			return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

			case EDescriptorType::UNIFORM_BUFFER:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

			case EDescriptorType::STORAGE_BUFFER:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

			case EDescriptorType::UNIFORM_DYNAMIC_BUFFER:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

			case EDescriptorType::STORAGE_DYNAMIC_BUFFER:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;

			case EDescriptorType::INPUT_ATTACHMENT:
			return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

			default:
			return VK_DESCRIPTOR_TYPE_MAX_ENUM;
		}
	}

	constexpr VkClearColorValue toClearColor(const ClearColor& kColor) noexcept
	{
		return { { kColor.mX, kColor.mY, kColor.mZ, kColor.mW } };
	}

	constexpr VkClearDepthStencilValue toClearDepthStencil(const ClearDepthStencil& kDepthStencil) noexcept
	{
		return { kDepthStencil.mDepth, kDepthStencil.mStencil };
	}

	constexpr VkSubpassDependency toSubpassDependency(const SubpassDependency& kDependency) noexcept
	{
		return { kDependency.mSrcState.mSubpassIndex, kDependency.mDstState.mSubpassIndex, static_cast<VkPipelineStageFlags>(toPipelineStageFlag(kDependency.mSrcState.mStageMask)),
				static_cast<VkPipelineStageFlags>(toPipelineStageFlag(kDependency.mDstState.mStageMask)), static_cast<VkAccessFlags>(toAccessFlag(kDependency.mSrcState.mAccessMask)),
				static_cast<VkAccessFlags>(toAccessFlag(kDependency.mDstState.mAccessMask)), kDependency.mByRegion ? VK_DEPENDENCY_BY_REGION_BIT : 0u };
	}

	constexpr VkVertexInputRate toVertexInputRate(EVertexInputRate rate) noexcept
	{
		switch (rate)
		{
			case EVertexInputRate::PER_INSTANCE:
			return VK_VERTEX_INPUT_RATE_INSTANCE;

			case EVertexInputRate::PER_VERTEX:
			default:
			return VK_VERTEX_INPUT_RATE_VERTEX;
		}
	}

	constexpr VkPrimitiveTopology toTopology(ETopology topo) noexcept
	{
		switch (topo)
		{
			case ETopology::LINE:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

			case ETopology::LINE_STRIP:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

			case ETopology::TRIANGLE:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

			case ETopology::TRIANGLE_STRIP:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

			case ETopology::TRIANGLE_FAN:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

			case ETopology::LINE_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;

			case ETopology::LINE_STRIP_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;

			case ETopology::TRIANGLE_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;

			case ETopology::TRIANGLE_STRIP_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;

			case ETopology::PATCH:
			return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

			case ETopology::POINT:
			default:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		}
	}

	constexpr VkPrimitiveTopology toNoAdjacentTopology(ETopology topo) noexcept
	{
		switch (topo)
		{
			case ETopology::LINE:
			case ETopology::LINE_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

			case ETopology::LINE_STRIP:
			case ETopology::LINE_STRIP_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

			case ETopology::TRIANGLE:
			case ETopology::TRIANGLE_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

			case ETopology::TRIANGLE_STRIP:
			case ETopology::TRIANGLE_STRIP_ADJACENCY:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

			case ETopology::TRIANGLE_FAN:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

			case ETopology::PATCH:
			return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

			case ETopology::POINT:
			default:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		}
	}

	constexpr bool hasAdjacency(ETopology topo) noexcept
	{
		switch (topo)
		{
			case ETopology::LINE_ADJACENCY:
			case ETopology::LINE_STRIP_ADJACENCY:
			case ETopology::TRIANGLE_ADJACENCY:
			case ETopology::TRIANGLE_STRIP_ADJACENCY:
			return true;

			default:
			return false;
		}
	}

	constexpr bool canRestartPrimitive(ETopology topo) noexcept
	{
		switch (topo)
		{
			case ETopology::LINE_STRIP:
			case ETopology::TRIANGLE_STRIP:
			case ETopology::TRIANGLE_FAN:
			case ETopology::LINE_STRIP_ADJACENCY:
			case ETopology::TRIANGLE_STRIP_ADJACENCY:
			return false;

			default:
			return true;
		}
	}

	constexpr VkPolygonMode toPolygonMode(EPolygonMode mode) noexcept
	{
		switch (mode)
		{
			case EPolygonMode::LINE:
			return VK_POLYGON_MODE_LINE;

			case EPolygonMode::POINT:
			return VK_POLYGON_MODE_POINT;

			case EPolygonMode::FILL:
			default:
			return VK_POLYGON_MODE_FILL;
		}
	}

	constexpr VkCullModeFlagBits toCullModeFlag(ECullModeFlag mode) noexcept
	{
		int32_t result { 0 };

		if ((mode & ECullModeFlag::FRONT) == ECullModeFlag::FRONT)
			result |= VK_CULL_MODE_FRONT_BIT;

		if ((mode & ECullModeFlag::BACK) == ECullModeFlag::BACK)
			result |= VK_CULL_MODE_BACK_BIT;

		return static_cast<VkCullModeFlagBits>(result);
	}

	constexpr VkCompareOp toCompareOp(ECompareOp op) noexcept
	{
		switch (op)
		{
			case ECompareOp::NEVER:
			return VK_COMPARE_OP_NEVER;

			case ECompareOp::ALWAYS:
			return VK_COMPARE_OP_ALWAYS;

			case ECompareOp::EQUAL:
			return VK_COMPARE_OP_EQUAL;

			case ECompareOp::NOT_EQUAL:
			return VK_COMPARE_OP_NOT_EQUAL;

			case ECompareOp::LESS:
			return VK_COMPARE_OP_LESS;

			case ECompareOp::GREATER:
			return VK_COMPARE_OP_GREATER;

			case ECompareOp::GREATER_EQUAL:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;

			case ECompareOp::LESS_EQUAL:
			default:
			return VK_COMPARE_OP_LESS_OR_EQUAL;
		}
	}

	constexpr VkStencilOp toStencilOp(EStencilOp op) noexcept
	{
		switch (op)
		{
			case EStencilOp::ZERO:
			return VK_STENCIL_OP_ZERO;

			case EStencilOp::REPLACE:
			return VK_STENCIL_OP_REPLACE;

			case EStencilOp::INVERT:
			return VK_STENCIL_OP_INVERT;

			case EStencilOp::INCREMENT_CLAMP:
			return VK_STENCIL_OP_INCREMENT_AND_CLAMP;

			case EStencilOp::DECREMENT_CLAMP:
			return VK_STENCIL_OP_DECREMENT_AND_CLAMP;

			case EStencilOp::INCREMENT_WRAP:
			return VK_STENCIL_OP_INCREMENT_AND_WRAP;

			case EStencilOp::DECREMENT_WRAP:
			return VK_STENCIL_OP_DECREMENT_AND_WRAP;

			case EStencilOp::KEEP:
			default:
			return VK_STENCIL_OP_KEEP;
		}
	}

	constexpr VkBlendFactor toBlendFactor(EBlendFactor factor) noexcept
	{
		switch (factor)
		{
			case EBlendFactor::ONE:
			return VK_BLEND_FACTOR_ONE;

			case EBlendFactor::SRC_RGB:
			return VK_BLEND_FACTOR_SRC_COLOR;

			case EBlendFactor::SRC_A:
			return VK_BLEND_FACTOR_SRC_ALPHA;

			case EBlendFactor::SRC_A_SATURATE:
			return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;

			case EBlendFactor::DST_RGB:
			return VK_BLEND_FACTOR_DST_COLOR;

			case EBlendFactor::DST_A:
			return VK_BLEND_FACTOR_DST_ALPHA;

			case EBlendFactor::CONSTANT_RGB:
			return VK_BLEND_FACTOR_CONSTANT_COLOR;

			case EBlendFactor::CONSTANT_A:
			return VK_BLEND_FACTOR_CONSTANT_ALPHA;

			case EBlendFactor::SRC1_RGB:
			return VK_BLEND_FACTOR_SRC1_COLOR;

			case EBlendFactor::SRC1_A:
			return VK_BLEND_FACTOR_SRC1_ALPHA;

			case EBlendFactor::ONE_MINUS_SRC_RGB:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;

			case EBlendFactor::ONE_MINUS_SRC_A:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

			case EBlendFactor::ONE_MINUS_DST_RGB:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;

			case EBlendFactor::ONE_MINUS_DST_A:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;

			case EBlendFactor::ONE_MINUS_CONSTANT_RGB:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;

			case EBlendFactor::ONE_MINUS_CONSTANT_A:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;

			case EBlendFactor::ONE_MINUS_SRC1_RGB:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;

			case EBlendFactor::ONE_MINUS_SRC1_A:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;

			case EBlendFactor::ZERO:
			default:
			return VK_BLEND_FACTOR_ZERO;
		}
	}

	constexpr EBlendFactor toUniqueSrcBlendFactor(EBlendFactor factor) noexcept
	{
		switch (factor)
		{
			case EBlendFactor::SRC1_RGB:
			return EBlendFactor::SRC_RGB;

			case EBlendFactor::SRC1_A:
			return EBlendFactor::SRC_A;

			case EBlendFactor::ONE_MINUS_SRC1_RGB:
			return EBlendFactor::ONE_MINUS_SRC_RGB;

			case EBlendFactor::ONE_MINUS_SRC1_A:
			return EBlendFactor::ONE_MINUS_SRC_A;

			default:
			return factor;
		}
	}

	constexpr bool usesDualSrcBlend(EBlendFactor factor) noexcept
	{
		switch (factor)
		{
			case EBlendFactor::SRC1_RGB:
			case EBlendFactor::SRC1_A:
			case EBlendFactor::ONE_MINUS_SRC1_RGB:
			case EBlendFactor::ONE_MINUS_SRC1_A:
			return true;

			default:
			return false;
		}
	}

	constexpr VkLogicOp toLogicOp(ELogicOp op) noexcept
	{
		switch (op)
		{
			case ELogicOp::CLEAR:
			return VK_LOGIC_OP_CLEAR;

			case ELogicOp::NO_OP:
			return VK_LOGIC_OP_NO_OP;

			case ELogicOp::COPY_INVERT:
			return VK_LOGIC_OP_COPY_INVERTED;

			case ELogicOp::INVERT:
			return VK_LOGIC_OP_INVERT;

			case ELogicOp::AND:
			return VK_LOGIC_OP_AND;

			case ELogicOp::OR:
			return VK_LOGIC_OP_OR;

			case ELogicOp::XOR:
			return VK_LOGIC_OP_XOR;

			case ELogicOp::NAND:
			return VK_LOGIC_OP_NAND;

			case ELogicOp::NOR:
			return VK_LOGIC_OP_NOR;

			case ELogicOp::NXOR:
			return VK_LOGIC_OP_EQUIVALENT;

			case ELogicOp::AND_REVERSE:
			return VK_LOGIC_OP_AND_REVERSE;

			case ELogicOp::OR_REVERSE:
			return VK_LOGIC_OP_OR_REVERSE;

			case ELogicOp::AND_INVERT:
			return VK_LOGIC_OP_AND_INVERTED;

			case ELogicOp::OR_INVERT:
			return VK_LOGIC_OP_OR_INVERTED;

			case ELogicOp::SET:
			return VK_LOGIC_OP_SET;

			case ELogicOp::COPY:
			default:
			return VK_LOGIC_OP_COPY;
		}
	}

	constexpr VkBlendOp toBlendOp(EBlendOp op) noexcept
	{
		switch (op)
		{
			case EBlendOp::SUBTRACT:
			return VK_BLEND_OP_SUBTRACT;

			case EBlendOp::REVERSE_SUBTRACT:
			return VK_BLEND_OP_REVERSE_SUBTRACT;

			case EBlendOp::MIN:
			return VK_BLEND_OP_MIN;

			case EBlendOp::MAX:
			return VK_BLEND_OP_MAX;

			case EBlendOp::ADD:
			default:
			return VK_BLEND_OP_ADD;
		}
	}

	constexpr VkColorComponentFlagBits toColorComponentFlag(EColorComponentFlag color) noexcept
	{
		int32_t result { 0 };

		if ((color & EColorComponentFlag::R) == EColorComponentFlag::R)
			result |= VK_COLOR_COMPONENT_R_BIT;

		if ((color & EColorComponentFlag::G) == EColorComponentFlag::G)
			result |= VK_COLOR_COMPONENT_G_BIT;

		if ((color & EColorComponentFlag::B) == EColorComponentFlag::B)
			result |= VK_COLOR_COMPONENT_B_BIT;

		if ((color & EColorComponentFlag::A) == EColorComponentFlag::A)
			result |= VK_COLOR_COMPONENT_A_BIT;

		return static_cast<VkColorComponentFlagBits>(result);
	}

	constexpr VkAttachmentReference toAttachmentRef(const AttachmentRef& kRef) noexcept
	{
		return { kRef.mIndex, toImageLayout(kRef.mLayout) };
	}
}

#endif