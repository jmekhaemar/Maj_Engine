#include "VkHelper.h"

#define CHECK_EXT_ENABLED(EXT_NAME, FEATURE_NAME)				\
		if (kExtensions.m##EXT_NAME == EExtensionLevel::ENABLED)	\
		{															\
			LOG(LogRHIInfo, "enabling feature: " << #EXT_NAME)		\
			features.FEATURE_NAME = true;						\
		}

#define RATE_DEVICE_EXT(EXT_NAME, FEATURE_NAME, REQUESTED_POINTS)								\
		if (kExtensions.m##EXT_NAME == EExtensionLevel::REQUIRED && !kFeatures.FEATURE_NAME)	\
		{																						\
			LOG(LogRHIWarning, "required extensions not supported: " << #EXT_NAME)				\
			return 0u;																			\
		}																						\
		else if (kExtensions.m##EXT_NAME != EExtensionLevel::IGNORED && kFeatures.FEATURE_NAME)	\
			score += REQUESTED_POINTS;

#define UPDATE_EXT(EXT_NAME, FEATURE_NAME)																			\
		if (extensions.m##EXT_NAME != EExtensionLevel::IGNORED)														\
			extensions.m##EXT_NAME = kFeatures.FEATURE_NAME ? EExtensionLevel::ENABLED : EExtensionLevel::DISABLED;

namespace maj::rhi
{
	VkStencilOpState toStencilState(const StencilState& kStencil) noexcept
	{
		return {	toStencilOp(kStencil.mFailOp), toStencilOp(kStencil.mPassOp), toStencilOp(kStencil.mDepthFailOp),
					toCompareOp(kStencil.mCompareOp), kStencil.mCompareMask,
					kStencil.mWriteMask, kStencil.mReference };
	}

	VkAttachmentDescription toAttachmentDescription(const AttachmentDescription& kDescription) noexcept
	{
		VkAttachmentDescription description {};
		description.format = toFormat(kDescription.mFormat);
		description.samples = toSampleFlag(kDescription.mSamples);
		description.initialLayout = toImageLayout(kDescription.mInitialLayout);
		description.finalLayout = toImageLayout(kDescription.mFinalLayout);

		EAttachmentOpFlag errorFlag { EAttachmentOpFlag::LOAD_COLOR | EAttachmentOpFlag::CLEAR_COLOR };
		if ((kDescription.mOp & errorFlag) == errorFlag)
			LOG(LogRHIWarning, "incoherence: contradictory flags in attachment description: can't load AND clear color/depth")

		errorFlag = EAttachmentOpFlag::LOAD_STENCIL | EAttachmentOpFlag::CLEAR_STENCIL;
		if ((kDescription.mOp & errorFlag) == errorFlag)
			LOG(LogRHIWarning, "incoherence: contradictory flags in attachment description: can't load AND clear stencil")

		description.loadOp = (kDescription.mOp & EAttachmentOpFlag::LOAD_COLOR) != EAttachmentOpFlag::DONT_CARE ? VK_ATTACHMENT_LOAD_OP_LOAD :
			((kDescription.mOp & EAttachmentOpFlag::CLEAR_COLOR) != EAttachmentOpFlag::DONT_CARE ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE);
		description.storeOp = (kDescription.mOp & EAttachmentOpFlag::STORE_COLOR) != EAttachmentOpFlag::DONT_CARE ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;

		description.stencilLoadOp = (kDescription.mOp & EAttachmentOpFlag::LOAD_STENCIL) != EAttachmentOpFlag::DONT_CARE ? VK_ATTACHMENT_LOAD_OP_LOAD :
			((kDescription.mOp & EAttachmentOpFlag::CLEAR_STENCIL) != EAttachmentOpFlag::DONT_CARE ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE);
		description.stencilStoreOp = (kDescription.mOp & EAttachmentOpFlag::STORE_STENCIL) != EAttachmentOpFlag::DONT_CARE ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;

		return description;
	}

	VkPhysicalDeviceFeatures toDeviceFeatures(const ExtensionList& kExtensions) noexcept
	{
		VkPhysicalDeviceFeatures features {};

		CHECK_EXT_ENABLED(CubeArray, imageCubeArray)
		CHECK_EXT_ENABLED(GeometryShader, geometryShader)
		CHECK_EXT_ENABLED(TesselerationShader, tessellationShader)
		CHECK_EXT_ENABLED(SampleRateShading, sampleRateShading)
		CHECK_EXT_ENABLED(DualSrcBlend, dualSrcBlend)
		CHECK_EXT_ENABLED(LogicOp, logicOp)
		CHECK_EXT_ENABLED(DepthClamp, depthClamp)
		CHECK_EXT_ENABLED(DepthBiasClamp, depthBiasClamp)
		CHECK_EXT_ENABLED(PolygonModeNonSolid, fillModeNonSolid)
		CHECK_EXT_ENABLED(DepthBounds, depthBounds)
		CHECK_EXT_ENABLED(AlphaToOne, alphaToOne)
		CHECK_EXT_ENABLED(MultiViewport, multiViewport)
		CHECK_EXT_ENABLED(SamplerAnisotropy, samplerAnisotropy)
		CHECK_EXT_ENABLED(ShaderClipDistance, shaderClipDistance)
		CHECK_EXT_ENABLED(ShaderCullDistance, shaderCullDistance)
		CHECK_EXT_ENABLED(ShaderFloat64, shaderFloat64)
		CHECK_EXT_ENABLED(ShaderInt64, shaderInt64)
		CHECK_EXT_ENABLED(ShaderInt16, shaderInt16)
		CHECK_EXT_ENABLED(ShaderResourceMinLOD, shaderResourceMinLod)
		CHECK_EXT_ENABLED(VariableMultiSamplingRate, variableMultisampleRate)
		
		return features;
	}

	uint32_t toDeviceScore(const VkPhysicalDeviceFeatures& kFeatures, const ExtensionList& kExtensions) noexcept
	{
		uint32_t score { 0u };

		RATE_DEVICE_EXT(CubeArray, imageCubeArray, 100u)
		RATE_DEVICE_EXT(GeometryShader, geometryShader, 100u)
		RATE_DEVICE_EXT(TesselerationShader, tessellationShader, 100u)
		RATE_DEVICE_EXT(SampleRateShading, sampleRateShading, 100u)
		RATE_DEVICE_EXT(DualSrcBlend, dualSrcBlend, 100u)
		RATE_DEVICE_EXT(LogicOp, logicOp, 100u)
		RATE_DEVICE_EXT(DepthClamp, depthClamp, 100u)
		RATE_DEVICE_EXT(DepthBiasClamp, depthBiasClamp, 100u)
		RATE_DEVICE_EXT(PolygonModeNonSolid, fillModeNonSolid, 100u)
		RATE_DEVICE_EXT(DepthBounds, depthBounds, 100u)
		RATE_DEVICE_EXT(AlphaToOne, alphaToOne, 100u)
		RATE_DEVICE_EXT(MultiViewport, multiViewport, 100u)
		RATE_DEVICE_EXT(SamplerAnisotropy, samplerAnisotropy, 100u)
		RATE_DEVICE_EXT(ShaderClipDistance, shaderClipDistance, 100u)
		RATE_DEVICE_EXT(ShaderCullDistance, shaderCullDistance, 100u)
		RATE_DEVICE_EXT(ShaderFloat64, shaderFloat64, 100u)
		RATE_DEVICE_EXT(ShaderInt64, shaderInt64, 100u)
		RATE_DEVICE_EXT(ShaderInt16, shaderInt16, 100u)
		RATE_DEVICE_EXT(ShaderResourceMinLOD, shaderResourceMinLod, 100u)
		RATE_DEVICE_EXT(VariableMultiSamplingRate, variableMultisampleRate, 100u)
		
		return score + 1u;
	}

	void updateExtensions(const VkPhysicalDeviceFeatures& kFeatures, ExtensionList& extensions) noexcept
	{
		UPDATE_EXT(CubeArray, imageCubeArray)
		UPDATE_EXT(GeometryShader, geometryShader)
		UPDATE_EXT(TesselerationShader, tessellationShader)
		UPDATE_EXT(SampleRateShading, sampleRateShading)
		UPDATE_EXT(DualSrcBlend, dualSrcBlend)
		UPDATE_EXT(LogicOp, logicOp)
		UPDATE_EXT(DepthClamp, depthClamp)
		UPDATE_EXT(DepthBiasClamp, depthBiasClamp)
		UPDATE_EXT(PolygonModeNonSolid, fillModeNonSolid)
		UPDATE_EXT(DepthBounds, depthBounds)
		UPDATE_EXT(AlphaToOne, alphaToOne)
		UPDATE_EXT(MultiViewport, multiViewport)
		UPDATE_EXT(SamplerAnisotropy, samplerAnisotropy)
		UPDATE_EXT(ShaderClipDistance, shaderClipDistance)
		UPDATE_EXT(ShaderCullDistance, shaderCullDistance)
		UPDATE_EXT(ShaderFloat64, shaderFloat64)
		UPDATE_EXT(ShaderInt64, shaderInt64)
		UPDATE_EXT(ShaderInt16, shaderInt16)
		UPDATE_EXT(ShaderResourceMinLOD, shaderResourceMinLod)
		UPDATE_EXT(VariableMultiSamplingRate, variableMultisampleRate)
	}
}