#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <Flag.h>
#include <MetaMacros.h>

namespace maj::rhi
{
	struct Format final
	{
		// R = Red, G = Green, B = Blue, A = Alpha
		// D = Depth, S = Stencil, E = Exponent, X = Unused
		enum class EComponent
		{
			UNDEFINED,

			// 8-bit format
			S8,					// UINT
			R8,					// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB
			R4G4,				// UNORM

			// 16-bit format
			D16,				// UNORM
			R16,				// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SFLOAT
			R8G8,				// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB
			R5G6B5,				// UNORM
			B5G6R5,				// UNORM
			R5G5B5A1,			// UNORM
			B5G5R5A1,			// UNORM
			A1R5G5B5,			// UNORM
			R4G4B4A4,			// UNORM
			B4G4R4A4,			// UNORM

			// 24-bit format
			D16S8,				// UNORM-UINT
			R8G8B8,				// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB
			B8G8R8,				// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB

			// 32-bit format
			D32,				// SFLOAT
			D24X8,				// UNORM
			D24S8,				// UNORM-UINT
			R32,				// UINT, SINT, SFLOAT
			R16G16,				// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SFLOAT
			B10G11R11,			// UFLOAT
			R8G8B8A8,			// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB
			B8G8R8A8,			// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB
			A8B8G8R8,			// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB
			E5B9G9R9,			// UFLOAT
			A2R10G10B10,		// UNORM, SNORM, USCALED, SSCALED, UINT, SINT
			A2B10G10R10,		// UNORM, SNORM, USCALED, SSCALED, UINT, SINT

			// 40-bit format
			D32S8,				// SFLOAT-UINT

			// 48-bit format
			R16G16B16,			// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SFLOAT

			// 64-bit format
			R64,				// UINT, SINT, SFLOAT
			R32G32,				// UINT, SINT, SFLOAT
			R16G16B16A16,		// UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SFLOAT

			// 96-bit format
			R32G32B32,			// UINT, SINT, SFLOAT

			// 128-bit format
			R64G64,				// UINT, SINT, SFLOAT
			R32G32B32A32,		// UINT, SINT, SFLOAT

			// 192-bit format
			R64G64B64,			// UINT, SINT, SFLOAT
			
			// 256-bit format
			R64G64B64A64,		// UINT, SINT, SFLOAT
		};

		enum class EType
		{
			UNORM,		// Float [0, 1]
			SNORM,		// Float [-1, 1]
			USCALED,	// Float [0, 256] (for 8 bit)
			SSCALED,	// Float [-128, 127] (for 8 bit)
			UINT,		// Integer [0, 256] (for 8 bit)
			SINT,		// Integer [-128, 127] (for 8 bit)
			UFLOAT,		// Float unsigned
			SFLOAT,		// Float signed
			SRGB		// Linear color space
		};

		EComponent mComponent;
		EType mType;
	};

	enum class EColorSpace
	{
		UNDEFINED,
		SRGB
	};

	enum class EFormatFeatureFlag : uint32_t
	{
		SAMPLED_IMAGE = 1u,
		UNIFORM_TEXEL_BUFFER = 2u,
		STORAGE_IMAGE = 4u,
		STORAGE_TEXEL_BUFFER = 8u,
		STORAGE_ATOMIC_IMAGE = 16u,
		STORAGE_ATOMIC_TEXEL_BUFFER = 32u,
		VERTEX_BUFFER = 64u,
		COLOR_ATTACHMENT = 128u,
		COLOR_ATTACHMENT_BLEND = 256u,
		DEPTH_STENCIL_ATTACHMENT = 512u,
		BLIT_SRC = 1024u,
		BLIT_DST = 2048u,
		TRANSFER_SRC = 4096u,
		TRANSFER_DST = 8192u,
		SAMPLED_FILTER_LINEAR = 16384u,
		SAMPLED_FILTER_CUBIC = 32768u,
		SAMPLED_FILTER_MINMAX = 65536u
	};

	DECLARE_FLAG(EFormatFeatureFlag)

	constexpr uint64_t toFormatSize(const Format& kFormat) noexcept;
	constexpr uint8_t toFormatComponentNumber(const Format& kFormat) noexcept;
}

META_ENUM(maj::rhi::Format::EComponent, 39u)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::UNDEFINED)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::S8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R4G4)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::D16)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R16)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R8G8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R5G6B5)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::B5G6R5)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R5G5B5A1)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::B5G5R5A1)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::A1R5G5B5)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R4G4B4A4)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::B4G4R4A4)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::D16S8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R8G8B8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::B8G8R8)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::D32)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::D24X8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::D24S8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R32)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R16G16)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::B10G11R11)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R8G8B8A8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::B8G8R8A8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::A8B8G8R8)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::E5B9G9R9)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::A2R10G10B10)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::A2B10G10R10)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::D32S8)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R16G16B16)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R64)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R32G32)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R16G16B16A16)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R32G32B32)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R64G64)
META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R32G32B32A32)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R64G64B64)

META_ENUM_VALUE(maj::rhi::Format::EComponent, maj::rhi::Format::EComponent::R64G64B64A64)

META_ENUM(maj::rhi::Format::EType, 9u)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::UNORM)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::SNORM)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::USCALED)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::SSCALED)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::UINT)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::SINT)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::UFLOAT)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::SFLOAT)
META_ENUM_VALUE(maj::rhi::Format::EType, maj::rhi::Format::EType::SRGB)

META_ENUM(maj::rhi::EColorSpace, 2u)
META_ENUM_VALUE(maj::rhi::EColorSpace, maj::rhi::EColorSpace::UNDEFINED)
META_ENUM_VALUE(maj::rhi::EColorSpace, maj::rhi::EColorSpace::SRGB)


#include "common/Format.inl"

#endif