#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <Flag.h>
#include <Vec4.h>

namespace maj::rhi
{
	enum class EPipelineStageFlag : uint16_t
	{
		TOP = 1u,
		DRAW_INDIRECT = 2u,
		VERTEX_INPUT = 4u,
		VERTEX_SHADER = 8u,
		TESS_CONTROL_SHADER = 16u,
		TESS_EVAL_SHADER = 32u,
		GEOMETRY_SHADER = 64u,
		FRAGMENT_SHADER = 128u,
		EARLY_FRAGMENT = 256u,
		LATE_FRAGMENT = 512u,
		COLOR_OUTPUT = 1024,
		BOTTOM = 2048u,
		COMPUTE_SHADER = 4096u,
		TRANSFER = 8192u,
		ALL_GRAPHICS = 4095u,
		ALL_COMMANDS = 16384u
	};

	enum class EAccessFlag : uint16_t
	{
		INDIRECT_READ = 1u,
		INDEX_READ = 2u,
		VERTEX_ATTRIB_READ = 4u,
		UNIFORM_READ = 8u,
		INPUT_READ = 16u,
		SHADER_READ = 32u,
		SHADER_WRITE = 64u,
		COLOR_READ = 128u,
		COLOR_WRITE = 256u,
		DEPTH_STENCIL_READ = 512u,
		DEPTH_STENCIL_WRITE = 1024u,
		TRANSFER_READ = 2048u,
		TRANSFER_WRITE = 4096u,
		MEMORY_READ = 8192u,
		MEMORY_WRITE = 16384u
	};

	enum class EShaderStageFlag : uint8_t
	{
		VERTEX = 1u,
		TESS_CONTROL = 2u,
		TESS_EVAL = 4u,
		GEOMETRY = 8u,
		FRAGMENT = 16u,
		COMPUTE = 32u,
		ALL_GRAPHICS = 31u,
		ALL = 63u
	};

	enum class EDescriptorType
	{
		SAMPLER,
		TEXTURE,
		INPUT_ATTACHMENT,
		IMAGE,
		UNIFORM_BUFFER,
		UNIFORM_TEXEL_BUFFER,
		UNIFORM_DYNAMIC_BUFFER,
		STORAGE_IMAGE,
		STORAGE_BUFFER,
		STORAGE_TEXEL_BUFFER,
		STORAGE_DYNAMIC_BUFFER
	};

	struct SubpassDependencyState final
	{
		uint32_t mSubpassIndex;
		EPipelineStageFlag mStageMask;
		EAccessFlag mAccessMask;
	};

	struct SubpassDependency final
	{
		SubpassDependency(SubpassDependencyState&& srcState, SubpassDependencyState&& dstState, bool byRegion = false);
		~SubpassDependency() = default;

		SubpassDependencyState mSrcState;
		SubpassDependencyState mDstState;
		bool mByRegion;
	};

	typedef math::Vec4f ClearColor;

	struct ClearDepthStencil final
	{
		float mDepth;
		uint32_t mStencil;
	};

	DECLARE_FLAG(EPipelineStageFlag);
	DECLARE_FLAG(EAccessFlag);
	DECLARE_FLAG(EShaderStageFlag);
}

META_FLAG(maj::rhi::EPipelineStageFlag, 16u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::TOP, 0u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::DRAW_INDIRECT, 1u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::VERTEX_INPUT, 2u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::VERTEX_SHADER, 3u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::TESS_CONTROL_SHADER, 4u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::TESS_EVAL_SHADER, 5u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::GEOMETRY_SHADER, 6u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::FRAGMENT_SHADER, 7u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::EARLY_FRAGMENT, 8u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::LATE_FRAGMENT, 9u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::COLOR_OUTPUT, 10u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::BOTTOM, 11u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::COMPUTE_SHADER, 12u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::TRANSFER, 13u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::ALL_GRAPHICS, 14u)
META_FLAG_VALUE(maj::rhi::EPipelineStageFlag, maj::rhi::EPipelineStageFlag::ALL_COMMANDS, 15u)

META_FLAG(maj::rhi::EAccessFlag, 13u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::INDIRECT_READ, 0u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::INDEX_READ, 1u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::VERTEX_ATTRIB_READ, 2u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::UNIFORM_READ, 3u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::INPUT_READ, 4u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::SHADER_READ, 5u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::SHADER_WRITE, 6u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::COLOR_READ, 7u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::COLOR_WRITE, 8u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::DEPTH_STENCIL_READ, 9u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::DEPTH_STENCIL_WRITE, 10u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::TRANSFER_READ, 11u)
META_FLAG_VALUE(maj::rhi::EAccessFlag, maj::rhi::EAccessFlag::TRANSFER_WRITE, 12u)

META_FLAG(maj::rhi::EShaderStageFlag, 6u)
META_FLAG_VALUE(maj::rhi::EShaderStageFlag, maj::rhi::EShaderStageFlag::VERTEX, 0u)
META_FLAG_VALUE(maj::rhi::EShaderStageFlag, maj::rhi::EShaderStageFlag::TESS_CONTROL, 1u)
META_FLAG_VALUE(maj::rhi::EShaderStageFlag, maj::rhi::EShaderStageFlag::TESS_EVAL, 2u)
META_FLAG_VALUE(maj::rhi::EShaderStageFlag, maj::rhi::EShaderStageFlag::GEOMETRY, 3u)
META_FLAG_VALUE(maj::rhi::EShaderStageFlag, maj::rhi::EShaderStageFlag::FRAGMENT, 4u)
META_FLAG_VALUE(maj::rhi::EShaderStageFlag, maj::rhi::EShaderStageFlag::COMPUTE, 5u)

META_ENUM(maj::rhi::EDescriptorType, 11u)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::SAMPLER)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::TEXTURE)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::INPUT_ATTACHMENT)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::IMAGE)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::UNIFORM_BUFFER)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::UNIFORM_TEXEL_BUFFER)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::UNIFORM_DYNAMIC_BUFFER)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::STORAGE_IMAGE)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::STORAGE_BUFFER)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::STORAGE_TEXEL_BUFFER)
META_ENUM_VALUE(maj::rhi::EDescriptorType, maj::rhi::EDescriptorType::STORAGE_DYNAMIC_BUFFER)

#include "Pipeline.inl"

#endif