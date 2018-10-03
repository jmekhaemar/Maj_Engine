#ifndef _VK_HELPER_H_
#define _VK_HELPER_H_

#include <vulkan/vulkan.h>

#include "common/Memory.h"
#include "common/AttachmentDescription.h"
#include "common/Material.h"
#include "common/ExtensionList.h"

namespace maj::rhi
{
	constexpr VkFormat toFormat(const Format& kFormat) noexcept;
	constexpr Format toFormat(const VkFormat kFormat) noexcept;
	
	constexpr VkFormatFeatureFlagBits toFormatFeatureFlag(EFormatFeatureFlag feature) noexcept;

	constexpr VkColorSpaceKHR toColorSpace(EColorSpace colorSpace) noexcept;
	constexpr EColorSpace toColorSpace(VkColorSpaceKHR colorSpace) noexcept;

	constexpr VkMemoryPropertyFlagBits toMemoryFlag(EMemoryFlag memory) noexcept;

	constexpr VkImageTiling toImageTiling(EImageTiling tiling) noexcept;
	constexpr VkImageType toImageType(EImageType type) noexcept;
	constexpr VkImageViewType toImageViewType(EImageType type) noexcept;

	constexpr VkImageUsageFlagBits toImageUsage(EImageUsageFlag usage) noexcept;
	constexpr VkImageAspectFlagBits toImageAspect(EImageUsageFlag usage) noexcept;

	constexpr VkSampleCountFlagBits toSampleFlag(ESampleFlag sample) noexcept;
	constexpr VkImageLayout toImageLayout(EImageLayout layout) noexcept;

	constexpr VkPipelineStageFlagBits toPipelineStageFlag(EPipelineStageFlag stage) noexcept;
	constexpr VkAccessFlagBits toAccessFlag(EAccessFlag access) noexcept;
	constexpr VkShaderStageFlagBits toShaderStageFlag(EShaderStageFlag stage) noexcept;

	constexpr VkDescriptorType toDescriptorType(EDescriptorType type) noexcept;
	constexpr VkClearColorValue toClearColor(const ClearColor& kColor) noexcept;
	constexpr VkClearDepthStencilValue toClearDepthStencil(const ClearDepthStencil& kDepthStencil) noexcept;

	constexpr VkSubpassDependency toSubpassDependency(const SubpassDependency& kDependency) noexcept;
	constexpr VkVertexInputRate toVertexInputRate(EVertexInputRate rate) noexcept;

	constexpr VkPrimitiveTopology toTopology(ETopology topo) noexcept;
	constexpr VkPrimitiveTopology toNoAdjacentTopology(ETopology topo) noexcept;
	constexpr bool hasAdjacency(ETopology topo) noexcept;
	constexpr bool canRestartPrimitive(ETopology topo) noexcept;

	constexpr VkPolygonMode toPolygonMode(EPolygonMode mode) noexcept;
	constexpr VkCullModeFlagBits toCullModeFlag(ECullModeFlag mode) noexcept;
	constexpr VkCompareOp toCompareOp(ECompareOp op) noexcept;

	constexpr VkStencilOp toStencilOp(EStencilOp op) noexcept;
	VkStencilOpState toStencilState(const StencilState& kStencil) noexcept;

	constexpr VkBlendFactor toBlendFactor(EBlendFactor factor) noexcept;
	constexpr EBlendFactor toUniqueSrcBlendFactor(EBlendFactor factor) noexcept;
	constexpr bool usesDualSrcBlend(EBlendFactor factor) noexcept;
	constexpr VkLogicOp toLogicOp(ELogicOp op) noexcept;
	constexpr VkBlendOp toBlendOp(EBlendOp op) noexcept;
	constexpr VkColorComponentFlagBits toColorComponentFlag(EColorComponentFlag color) noexcept;

	constexpr VkAttachmentReference toAttachmentRef(const AttachmentRef& kRef) noexcept;
	VkAttachmentDescription toAttachmentDescription(const AttachmentDescription& kDescription) noexcept;

	VkPhysicalDeviceFeatures toDeviceFeatures(const ExtensionList& kExtensions) noexcept;
	uint32_t toDeviceScore(const VkPhysicalDeviceFeatures& kFeatures, const ExtensionList& kExtensions) noexcept;
	void updateExtensions(const VkPhysicalDeviceFeatures& kFeatures, ExtensionList& extensions) noexcept;
}

#include "VkHelper.inl"

#endif