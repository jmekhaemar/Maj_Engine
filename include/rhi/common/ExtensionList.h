#ifndef _RHI_EXTENSION_LIST_H_
#define _RHI_EXTENSION_LIST_H_

#include <ISerializable.h>

namespace maj::rhi
{
	enum class EExtensionLevel
	{
		DISABLED,
		REQUESTED,
		ENABLED,
		IGNORED = DISABLED,
		REQUIRED = ENABLED
	};

	struct ExtensionList final : public ISerializable
	{
		ENABLE_META(ExtensionList)
		
	public:
		LOAD_CONSTRUCTOR(ExtensionList)
		ExtensionList();

		EExtensionLevel mCubeArray;
		EExtensionLevel mGeometryShader;
		EExtensionLevel mTesselerationShader;
		EExtensionLevel mSampleRateShading;
		EExtensionLevel mDualSrcBlend;
		EExtensionLevel mLogicOp;
		EExtensionLevel mDepthClamp;
		EExtensionLevel mDepthBiasClamp;
		EExtensionLevel mPolygonModeNonSolid;
		EExtensionLevel mDepthBounds;
		EExtensionLevel mAlphaToOne;
		EExtensionLevel mMultiViewport;
		EExtensionLevel mSamplerAnisotropy;
		EExtensionLevel mShaderClipDistance;
		EExtensionLevel mShaderCullDistance;
		EExtensionLevel mShaderFloat64;
		EExtensionLevel mShaderInt64;
		EExtensionLevel mShaderInt16;
		EExtensionLevel mShaderResourceMinLOD;
		EExtensionLevel mVariableMultiSamplingRate;
	};
}

META_CLASS(maj::rhi::ExtensionList, 20u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mCubeArray, 0u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mGeometryShader, 1u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mTesselerationShader, 2u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mSampleRateShading, 3u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mDualSrcBlend, 4u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mLogicOp, 5u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mDepthClamp, 6u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mDepthBiasClamp, 7u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mPolygonModeNonSolid, 8u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mDepthBounds, 9u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mAlphaToOne, 10u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mMultiViewport, 11u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mSamplerAnisotropy, 12u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mShaderClipDistance, 13u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mShaderCullDistance, 14u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mShaderFloat64, 15u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mShaderInt64, 16u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mShaderInt16, 17u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mShaderResourceMinLOD, 18u)
META_FIELD(maj::rhi::ExtensionList, maj::rhi::EExtensionLevel, mVariableMultiSamplingRate, 19u)

META_ENUM(maj::rhi::EExtensionLevel, 3u)
META_ENUM_VALUE(maj::rhi::EExtensionLevel, maj::rhi::EExtensionLevel::DISABLED)
META_ENUM_VALUE(maj::rhi::EExtensionLevel, maj::rhi::EExtensionLevel::REQUESTED)
META_ENUM_VALUE(maj::rhi::EExtensionLevel, maj::rhi::EExtensionLevel::ENABLED)
//META_ENUM_VALUE(maj::rhi::EExtensionLevel, maj::rhi::EExtensionLevel::IGNORED)
//META_ENUM_VALUE(maj::rhi::EExtensionLevel, maj::rhi::EExtensionLevel::REQUIRED)

#endif