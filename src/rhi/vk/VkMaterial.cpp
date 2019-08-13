#include "Material.h"

#include <Window.h>

#include "common/Logging.h"

#include "Device.h"
#include "Instance.h"
#include "Renderer.h"
#include "VkHelper.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(Material::Info)
	DEFINE_HASH_OVERLOAD(Material)

	Material::Info::Info(JsonNode* root)
		: mSubpassIndex { 0u }, mSortingOrder { 0u }, mShaders {}, mPipelineState {}, mkRenderPass { &Service::get<Renderer>()->swapChain(0u).renderPass() }
	{
		load(root);
		initialize();
	}
	
	Material::Info::Info()
		: mSubpassIndex { 0u }, mSortingOrder { 127u }, mShaders {}, mPipelineState {}, mkRenderPass { &Service::get<Renderer>()->swapChain(0u).renderPass() }
	{}

	Material::Info::Info(const RenderPass& kRenderPass, uint32_t subpassIndex, uint32_t sortingOrder)
		: mSubpassIndex { subpassIndex }, mSortingOrder { sortingOrder }, mShaders {}, mPipelineState {}, mkRenderPass { &kRenderPass }
	{}

	Material::Info::Info(const Info& kOther, const RenderPass& kRenderPass, uint32_t subpassIndex)
		: mSubpassIndex { subpassIndex }, mSortingOrder { kOther.mSortingOrder }, mShaders { kOther.mShaders }, mPipelineState { kOther.mPipelineState }, mkRenderPass { &kRenderPass }
	{}

	Material::Info::Info(Info&& other)
		: mSubpassIndex { other.mSubpassIndex }, mSortingOrder { other.mSortingOrder }, mShaders { std::move(other.mShaders) },
		mPipelineState { std::move(other.mPipelineState) }, mkRenderPass { other.mkRenderPass }
	{}

	void Material::Info::initialize()
	{}

	const RenderPass& Material::Info::renderPass() const
	{
		return *mkRenderPass;
	}

	std::vector<VkPipelineShaderStageCreateInfo> Material::Info::shaderStagesInfos() const
	{
		std::vector<VkPipelineShaderStageCreateInfo> flags {};
		uint32_t shaderCount { static_cast<uint32_t>(mShaders.size()) };
		flags.reserve(shaderCount);

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		for (uint32_t i { 0u } ; i < shaderCount ; ++i)
		{
			VkPipelineShaderStageCreateInfo info { mShaders[i].info() };
			if ((info.stage & VK_SHADER_STAGE_GEOMETRY_BIT) == VK_SHADER_STAGE_GEOMETRY_BIT && kExtensions.mGeometryShader == EExtensionLevel::DISABLED)
			{
				LOG(LogRHIWarning, "missing device extension: geometry shader not enabled")
				continue;
			}

			if (((info.stage & VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) == VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
				|| (info.stage & VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) == VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) && 
				kExtensions.mTesselerationShader == EExtensionLevel::DISABLED)
			{
				LOG(LogRHIWarning, "failed to get shader info: tesseleration shader not enabled")
				continue;
			}

			flags.emplace_back(std::move(info));
		}

		return flags;
	}

	VkPipelineInputAssemblyStateCreateInfo Material::Info::inputAssemblyInfo() const
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssembly { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		if (hasAdjacency(mPipelineState.mTopology) && kExtensions.mGeometryShader == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: geometry shader not enabled")
			inputAssembly.topology = toNoAdjacentTopology(mPipelineState.mTopology);
		}
		else if (mPipelineState.mTopology == ETopology::PATCH && kExtensions.mTesselerationShader == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: tesseleration shader not enabled")
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		}
		else
			inputAssembly.topology = toTopology(mPipelineState.mTopology);

		if (mPipelineState.mPrimitiveRestartEnabled && !canRestartPrimitive(mPipelineState.mTopology))
		{
			LOG(LogRHIWarning, "incoherence: primitive topology cannot be restarted: topology with 'strip' or 'fan' assembly")
			inputAssembly.primitiveRestartEnable = false;
		}
		inputAssembly.primitiveRestartEnable = mPipelineState.mPrimitiveRestartEnabled;

		return inputAssembly;
	}

	VkPipelineRasterizationStateCreateInfo Material::Info::rasterizationInfo() const
	{
		VkPipelineRasterizationStateCreateInfo rasterizer { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		if (mPipelineState.mDepthClampEnabled && kExtensions.mDepthClamp == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: depth clamp not enabled")
			rasterizer.depthClampEnable = false;
		}
		else
			rasterizer.depthClampEnable = mPipelineState.mDepthClampEnabled;

		rasterizer.rasterizerDiscardEnable = VK_FALSE;								// Always false

		if (mPipelineState.mPolygonMode != EPolygonMode::FILL && kExtensions.mPolygonModeNonSolid == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: polygon mode non solid not enabled")
			rasterizer.depthClampEnable = toPolygonMode(EPolygonMode::FILL);
		}
		else
			rasterizer.polygonMode = toPolygonMode(mPipelineState.mPolygonMode);

		rasterizer.lineWidth = 1.f;													// Always 1
		rasterizer.cullMode = toCullModeFlag(mPipelineState.mCullMode);
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;								// Always Clockwise
		rasterizer.depthBiasEnable = mPipelineState.mDepthBias.isEnabled();
		rasterizer.depthBiasConstantFactor = mPipelineState.mDepthBias.mConstantFactor;

		if (mPipelineState.mDepthBias.mClamp != 0.f && kExtensions.mDepthBiasClamp == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: depth bias clamp not enabled")
			rasterizer.depthBiasClamp = 0.f;
		}
		else
			rasterizer.depthBiasClamp = mPipelineState.mDepthBias.mClamp;

		rasterizer.depthBiasSlopeFactor = mPipelineState.mDepthBias.mSlopeFactor;

		return rasterizer;
	}

	VkPipelineMultisampleStateCreateInfo Material::Info::multisamplingInfo() const
	{
		VkPipelineMultisampleStateCreateInfo multisampling { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		if (mPipelineState.mSampleState.mMinSampleShading < 1.f && kExtensions.mSampleRateShading == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: sample rate shading not enabled")
			multisampling.sampleShadingEnable = false;
		}
		else
			multisampling.sampleShadingEnable = mPipelineState.mSampleState.mMinSampleShading < 1.f;
		
		multisampling.rasterizationSamples = toSampleFlag(mPipelineState.mSampleState.mSampleCount);
		multisampling.minSampleShading = std::clamp(mPipelineState.mSampleState.mMinSampleShading, 0.f, 1.f);
		multisampling.pSampleMask = mPipelineState.mSampleState.mSampleMaskEnabled ? &mPipelineState.mSampleState.mSampleMask : nullptr;
		multisampling.alphaToCoverageEnable = mPipelineState.mAlphaToCoverageEnabled;
		
		if (mPipelineState.mAlphaToOneEnabled && kExtensions.mAlphaToOne == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: alpha to one not enabled")
			multisampling.alphaToOneEnable = false;
		}
		else
			multisampling.alphaToOneEnable = mPipelineState.mAlphaToOneEnabled;

		return multisampling;
	}

	VkPipelineDepthStencilStateCreateInfo Material::Info::depthStencilInfo() const
	{
		VkPipelineDepthStencilStateCreateInfo depthStencil { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
		depthStencil.depthTestEnable = mPipelineState.mDepthTestEnabled;
		depthStencil.depthWriteEnable = mPipelineState.mDepthWriteEnabled;
		depthStencil.depthCompareOp = toCompareOp(mPipelineState.mDepthCompareOp);

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		bool depthBoundEnabled { mPipelineState.mDepthBound.isEnabled() };
		if (depthBoundEnabled && kExtensions.mDepthBounds == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: depth bounds not enabled")
			depthStencil.depthBoundsTestEnable = false;
		}
		else
			depthStencil.depthBoundsTestEnable = depthBoundEnabled;

		depthStencil.minDepthBounds = mPipelineState.mDepthBound.mMin;
		depthStencil.maxDepthBounds = mPipelineState.mDepthBound.mMax;
		depthStencil.stencilTestEnable = mPipelineState.mFrontStencil.isEnabled() || mPipelineState.mBackStencil.isEnabled();
		depthStencil.front = toStencilState(mPipelineState.mFrontStencil);
		depthStencil.back = toStencilState(mPipelineState.mBackStencil);

		return depthStencil;
	}
	
	Material::Info& Material::Info::operator=(Info&& other)
	{
		mkRenderPass = other.mkRenderPass;
		mSubpassIndex = other.mSubpassIndex;
		mSortingOrder = other.mSortingOrder;
		mShaders = std::move(other.mShaders);
		mPipelineState = std::move(other.mPipelineState);

		return *this;
	}

	Material::VertexInputState::VertexInputState(const Info& kInfo)
		: mBindings {}, mAttributes {}
	{
		uint32_t bindingCount { static_cast<uint32_t>(kInfo.mPipelineState.mBindings.size()) };
		if (bindingCount == 0u)
			return;

		mBindings.reserve(bindingCount);
		for (uint32_t i { 0u } ; i < bindingCount ; ++i)
		{
			const VertexBinding& kBinding { kInfo.mPipelineState.mBindings[i] };

			uint32_t stride { 0u };
			uint32_t attributeCount { static_cast<uint32_t>(kBinding.mAttributes.size()) };
			for (uint32_t j { 0u } ; j < attributeCount ; ++j)
			{
				const VertexAttribute& kAttribute { kBinding.mAttributes[j] };
				VkVertexInputAttributeDescription attrib { j, kBinding.mBinding, toFormat(kAttribute.format()), stride };
				mAttributes.emplace_back(std::move(attrib));

				stride += kAttribute.mSize;
			}

			VkVertexInputBindingDescription bind { kBinding.mBinding, stride, toVertexInputRate(kBinding.mInputRate) };
			mBindings.emplace_back(std::move(bind));
		}
	}

	VkPipelineVertexInputStateCreateInfo Material::VertexInputState::vertexInputInfo() const
	{
		VkPipelineVertexInputStateCreateInfo vertexInput { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
		vertexInput.vertexBindingDescriptionCount = static_cast<uint32_t>(mBindings.size());
		vertexInput.pVertexBindingDescriptions = mBindings.data();
		vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(mAttributes.size());
		vertexInput.pVertexAttributeDescriptions = mAttributes.data();

		return vertexInput;
	}

	Material::ColorBlendState::ColorBlendState(const Info& kInfo)
		: mAttachments {}, mLogicOp { kInfo.mPipelineState.mLogicOp }, mBlendConstants { kInfo.mPipelineState.mBlendConstants }
	{
		uint32_t attachmentCount { kInfo.renderPass().countAttachments(EAttachmentUsageFlag::COLOR) };
		if (attachmentCount == 0u)
			return;

		const BlendState& kColorState { kInfo.mPipelineState.mColorBlend };
		const BlendState& kAlphaState { kInfo.mPipelineState.mAlphaBlend };

		bool blendEnabled { kColorState.isEnabled() || kAlphaState.isEnabled() };
		EBlendFactor blendFactors[4u] {	kColorState.mSrcFactor, kColorState.mDstFactor, kAlphaState.mSrcFactor, kAlphaState.mDstFactor };

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		if (kExtensions.mDualSrcBlend == EExtensionLevel::DISABLED)
		{
			bool usesDualSrc { false };
			for (uint8_t i { 0u } ; i < 4u ; ++i)
			{
				if (!usesDualSrcBlend(blendFactors[i]))
					continue;

				blendFactors[i] = toUniqueSrcBlendFactor(blendFactors[i]);
				usesDualSrc = true;
			}

			if (usesDualSrc)
				LOG(LogRHIWarning, "missing device extension: dual source blending not enabled");
		}

		VkPipelineColorBlendAttachmentState state { blendEnabled, toBlendFactor(blendFactors[0u]), toBlendFactor(blendFactors[1u]), toBlendOp(kColorState.mBlendOp),
													toBlendFactor(blendFactors[2u]), toBlendFactor(blendFactors[3u]), toBlendOp(kAlphaState.mBlendOp),
													static_cast<VkColorComponentFlags>(toColorComponentFlag(kInfo.mPipelineState.mColorMask)) };
		
		mAttachments.resize(attachmentCount, state);
	}

	VkPipelineColorBlendStateCreateInfo Material::ColorBlendState::colorBlendInfo() const
	{
		VkPipelineColorBlendStateCreateInfo colorBlending { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		bool logicOpEnabled = mLogicOp != ELogicOp::COPY;
		if (logicOpEnabled && kExtensions.mLogicOp == EExtensionLevel::DISABLED)
		{
			LOG(LogRHIWarning, "missing device extension: logic op not enabled")
			colorBlending.logicOpEnable = false;
		}
		else
			colorBlending.logicOpEnable = logicOpEnabled;

		colorBlending.logicOp = toLogicOp(mLogicOp);

		colorBlending.attachmentCount = static_cast<uint32_t>(mAttachments.size());
		colorBlending.pAttachments = mAttachments.data();

		colorBlending.blendConstants[0u] = mBlendConstants.mR;
		colorBlending.blendConstants[1u] = mBlendConstants.mG;
		colorBlending.blendConstants[2u] = mBlendConstants.mB;
		colorBlending.blendConstants[3u] = mBlendConstants.mA;

		return colorBlending;
	}

	Material::Material(JsonNode* root)
		: mInfo {}, mMaterialLayout {}, mTextures {}, mPipelineLayout { VK_NULL_HANDLE }, mPipeline { VK_NULL_HANDLE }
	{
		load(root);
		initialize();
	}

	Material::Material(const MaterialLayout* kMaterialLayout, Info&& info)
		: mInfo { std::move(info) }, mMaterialLayout { kMaterialLayout }, mTextures {}, mPipelineLayout { VK_NULL_HANDLE }, mPipeline { VK_NULL_HANDLE }
	{
		initialize();
	}

	Material::Material(const Material& kOther, Info&& info, const MaterialLayout* kMaterialLayout)
		: mInfo { std::move(info) }, mMaterialLayout { kMaterialLayout ? kMaterialLayout : kOther.mMaterialLayout.get(), false }, mTextures { kOther.mTextures },
		mPipelineLayout { VK_NULL_HANDLE }, mPipeline { kOther.mPipeline }
	{
		initialize();
	}

	Material::~Material()
	{
		Service::get<Renderer>()->wait();

		const Device* kDevice { Service::get<Device>() };

		if (mPipeline)
			vkDestroyPipeline(kDevice->device(), mPipeline, nullptr);

		if (mPipelineLayout)
			vkDestroyPipelineLayout(kDevice->device(), mPipelineLayout, nullptr);
	}

	void Material::initialize()
	{
		if (mOnChange.empty())
		{
			mOnChange += [this] () -> void
			{
				Renderer* renderer { Service::get<Renderer>() };
				const Device* kDevice { Service::get<Device>() };
				renderer->wait();

				if (mPipeline)
					vkDestroyPipeline(kDevice->device(), mPipeline, nullptr);

				if (mPipelineLayout)
					vkDestroyPipelineLayout(kDevice->device(), mPipelineLayout, nullptr);

				if (mMaterialLayout.empty())
					THROW("failed to create material: no material layout")

				createPipelineLayout({});

				const ww::Viewport& kViewport { Service::get<ww::Window>()->viewport(0u) };
				createPipeline(kViewport.extent(), VK_NULL_HANDLE);

				renderer->frameGraph().dirty(this);
			};
		}

		if (mMaterialLayout.empty())
			THROW("failed to create material: no material layout")

		mMaterialLayout.onChange([this] () -> void { mOnChange(); });

		for (TextureMap::iterator it { mTextures.begin() } ; it != mTextures.end() ; ++it)
			it->second.onChange([this] () -> void { mOnChange(); });

		createPipelineLayout({});

		VkPipeline parentPipeline { mPipeline };
		mPipeline = VK_NULL_HANDLE;

		const ww::Viewport& kViewport { Service::get<ww::Window>()->viewport(0u) };
		createPipeline(kViewport.extent(), parentPipeline);
	}

	void Material::bindPipeline(VkCommandBuffer commandBuffer) const
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
	}

	void Material::bindDescriptors(VkCommandBuffer commandBuffer, const std::map<uint32_t, std::vector<VkDescriptorSet>>& kSets) const
	{
		for (const std::pair<const uint32_t, std::vector<VkDescriptorSet>>& kPair : kSets)
		{
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, kPair.first,
									static_cast<uint32_t>(kPair.second.size()), kPair.second.data(), 0u, nullptr);
		}
	}

	const Material::Info& Material::info() const
	{
		return mInfo;
	}

	const MaterialLayout& Material::layout() const
	{
		return *mMaterialLayout;
	}

	const Texture* Material::texture(uint32_t setIndex, uint32_t bindingIndex) const
	{
		MaterialBinding coord { setIndex, bindingIndex };
		if (mTextures.count(coord) == 0u)
			return nullptr;

		return mTextures.at(coord).get();
	}

	void Material::texture(uint32_t setIndex, uint32_t bindingIndex, const Texture* kTexture)
	{
		const LayoutBinding* kBinding { mMaterialLayout->binding(setIndex, bindingIndex) };
		if (!kBinding || kBinding->mType != EDescriptorType::TEXTURE)
		{
			LOG(LogRHIWarning, "failed to set texture: descriptor " << setIndex << "." << bindingIndex << " not found or not a texture");
			return;
		}

		MaterialBinding coord { setIndex, bindingIndex };
		if (mTextures.count(coord) == 0u)
			mTextures.emplace(coord, kTexture);
		else
			mTextures[coord] = kTexture;

		mTextures[coord].onChange([this]() -> void { mOnChange(); });
	}

	void Material::createPipelineLayout(const std::vector<VkPushConstantRange>& kPushConstantRanges)
	{
		const std::vector<MaterialLayout::Layout>& kLayouts { mMaterialLayout->pipelineLayouts() };
		uint32_t setCount { static_cast<uint32_t>(kLayouts.size()) };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
		pipelineLayoutInfo.setLayoutCount = setCount;
		pipelineLayoutInfo.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>(kLayouts.data());
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(kPushConstantRanges.size());
		pipelineLayoutInfo.pPushConstantRanges = kPushConstantRanges.data();
		
		VkResult result { vkCreatePipelineLayout(Service::get<Device>()->device(), &pipelineLayoutInfo, nullptr, &mPipelineLayout) };
		if (result != VK_SUCCESS)
			THROW("failed to create pipeline layout with error: " + std::to_string(result));
	}

	void Material::createPipeline(const math::Vec2ui& kExtent, VkPipeline parentPipeline)
	{
		// Shader stages

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages { mInfo.shaderStagesInfos() };
		uint32_t shaderCount { static_cast<uint32_t>(shaderStages.size()) };

		VertexInputState vertexInputState { mInfo };
		ColorBlendState colorBlendState { mInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo { vertexInputState.vertexInputInfo() };
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo { mInfo.inputAssemblyInfo() };
		VkPipelineRasterizationStateCreateInfo rasterizationInfo { mInfo.rasterizationInfo() };
		VkPipelineMultisampleStateCreateInfo multisamplingInfo { mInfo.multisamplingInfo() };
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo { mInfo.depthStencilInfo() };
		VkPipelineColorBlendStateCreateInfo colorBlending { colorBlendState.colorBlendInfo() };

		// Viewport transformations

		VkViewport viewport { 0.f, 0.f, static_cast<float>(kExtent.mX), static_cast<float>(kExtent.mY), 0.f, 1.f };
		VkRect2D scissor { { 0, 0 }, { kExtent.mX, kExtent.mY } };

		VkPipelineViewportStateCreateInfo viewportState { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
		viewportState.viewportCount = 1u;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1u;
		viewportState.pScissors = &scissor;

		// Dynamic states

		VkDynamicState dynamicStates[] {	VK_DYNAMIC_STATE_VIEWPORT,
											VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicState { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
		dynamicState.dynamicStateCount = 2u;
		dynamicState.pDynamicStates = dynamicStates;

		// Pipeline creation

		VkGraphicsPipelineCreateInfo pipelineInfo { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
		pipelineInfo.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
		if (parentPipeline)
			pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;

		pipelineInfo.stageCount = shaderCount;
		pipelineInfo.pStages = shaderStages.data();

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizationInfo;
		pipelineInfo.pMultisampleState = &multisamplingInfo;
		pipelineInfo.pDepthStencilState = &depthStencilInfo;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;

		pipelineInfo.layout = mPipelineLayout;
		pipelineInfo.renderPass = mInfo.renderPass().renderPass();
		pipelineInfo.subpass = mInfo.mSubpassIndex;
		pipelineInfo.basePipelineHandle = parentPipeline;
		pipelineInfo.basePipelineIndex = -1;

		VkResult result { vkCreateGraphicsPipelines(Service::get<Device>()->device(), VK_NULL_HANDLE, 1u, &pipelineInfo, nullptr, &mPipeline) };
		if (result != VK_SUCCESS)
			THROW("failed to create graphics pipeline with error: " + std::to_string(result))
	}
}