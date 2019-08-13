#ifndef _RHI_MEMORY_H_
#define _RHI_MEMORY_H_

#include <cstdint>

#include <Flag.h>
#include <MetaMacros.h>

namespace maj::rhi
{
	enum class EMemoryFlag : uint8_t
	{
		DEVICE_LOCAL = 1u,
		HOST_VISIBLE = 2u,
		HOST_COHERENT = 4u,
		HOST_CACHED = 8u,
		LAZY_ALLOC = 16u
	};
}

#include "common/Memory.inl"

META_FLAG(maj::rhi::EMemoryFlag, 5u)
META_FLAG_VALUE(maj::rhi::EMemoryFlag, maj::rhi::EMemoryFlag::DEVICE_LOCAL, 0u)
META_FLAG_VALUE(maj::rhi::EMemoryFlag, maj::rhi::EMemoryFlag::HOST_VISIBLE, 1u)
META_FLAG_VALUE(maj::rhi::EMemoryFlag, maj::rhi::EMemoryFlag::HOST_COHERENT, 2u)
META_FLAG_VALUE(maj::rhi::EMemoryFlag, maj::rhi::EMemoryFlag::HOST_CACHED, 3u)
META_FLAG_VALUE(maj::rhi::EMemoryFlag, maj::rhi::EMemoryFlag::LAZY_ALLOC, 4u)

#endif