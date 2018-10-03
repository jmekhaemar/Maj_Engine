#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <vector>
#include <map>

#include <NonCopyable.h>
#include <Vec2.h>

#include "common/Material.h"

#include "Shader.h"
#include "RenderPass.h"
#include "MaterialLayout.h"
#include "Texture.h"

namespace maj::rhi
{
	class Material final : public NonCopyable, public ISerializable
	{
		ENABLE_META(Material)

	public:
		typedef std::map<MaterialBinding, Asset<Texture>> TextureMap;

		struct Info final : public NonCopyable, public ISerializable
		{
			ENABLE_META(Info)

		public:
			LOAD_CONSTRUCTOR_CUSTOM(Info)

			Info();
			Info(const RenderPass& kRenderPass, uint32_t subpassIndex = 0u, uint32_t sortingOrder = 127u);
			Info(const Info& kOther, const RenderPass& kRenderPass, uint32_t subpassIndex);
			Info(Info&& other);
			~Info() = default;

			const RenderPass& renderPass() const;

			std::vector<VkPipelineShaderStageCreateInfo> shaderStagesInfos() const;
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo() const;
			VkPipelineRasterizationStateCreateInfo rasterizationInfo() const;
			VkPipelineMultisampleStateCreateInfo multisamplingInfo() const;
			VkPipelineDepthStencilStateCreateInfo depthStencilInfo() const;

			Info& operator=(Info&& other);

			uint32_t mSubpassIndex;
			uint32_t mSortingOrder;

			std::vector<Shader> mShaders;
			PipelineState mPipelineState;

		private:
			const RenderPass* mkRenderPass;
		};

	private:
		struct VertexInputState final
		{
			VertexInputState(const Info& kInfo);
			~VertexInputState() = default;

			VkPipelineVertexInputStateCreateInfo vertexInputInfo() const;

			std::vector<VkVertexInputBindingDescription> mBindings;
			std::vector<VkVertexInputAttributeDescription> mAttributes;
		};

		struct ColorBlendState final
		{
			ColorBlendState(const Info& kInfo);
			~ColorBlendState() = default;

			VkPipelineColorBlendStateCreateInfo colorBlendInfo() const;

			std::vector<VkPipelineColorBlendAttachmentState> mAttachments;
			ELogicOp mLogicOp;
			BlendConstant mBlendConstants;
		};

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Material)

		Material(const MaterialLayout* kMaterialLayout, Info&& info);
		Material(const Material& kOther, Info&& info, const MaterialLayout* kMaterialLayout = nullptr);
		~Material();

		void bindPipeline(VkCommandBuffer commandBuffer) const;
		void bindDescriptors(VkCommandBuffer commandBuffer, const std::map<uint32_t, std::vector<VkDescriptorSet>>& kSets) const;

		const Info& info() const;
		const MaterialLayout& layout() const;

		const Texture* texture(uint32_t set, uint32_t binding) const;
		void texture(uint32_t set, uint32_t binding, const Texture* kTexture);
 
		Event<void> mOnChange;

	private:
		void createPipelineLayout(const std::vector<VkPushConstantRange>& kPushConstantRanges);
		void createPipeline(const math::Vec2ui& kExtent, VkPipeline parentPipeline);

		Info mInfo;

		Asset<MaterialLayout> mMaterialLayout;
		TextureMap mTextures;

		VkPipelineLayout mPipelineLayout;
		VkPipeline mPipeline;
	};
}

META_CLASS(maj::rhi::Material::Info, 4u)
META_FIELD(maj::rhi::Material::Info, uint32_t, mSubpassIndex, 0u)
META_FIELD(maj::rhi::Material::Info, uint32_t, mSortingOrder, 1u)
META_FIELD_ARRAY(maj::rhi::Material::Info, std::vector<maj::rhi::Shader>, mShaders, 2u)
META_FIELD(maj::rhi::Material::Info, maj::rhi::PipelineState, mPipelineState, 3u)

META_CLASS(maj::Asset<maj::rhi::Material>, 1u)
META_FIELD(maj::Asset<maj::rhi::Material>, uint32_t, mGuid, 0u)

META_CLASS(maj::rhi::Material, 4u)
META_FIELD(maj::rhi::Material, uint32_t, mGuid, 0u)
META_FIELD(maj::rhi::Material, maj::rhi::Material::Info, mInfo, 1u)
META_FIELD(maj::rhi::Material, maj::Asset<maj::rhi::MaterialLayout>, mMaterialLayout, 2u)
META_FIELD_MAP(maj::rhi::Material, maj::rhi::Material::TextureMap, mTextures, 3u)

#endif