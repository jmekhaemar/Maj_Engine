#include "RenderPass.h"

#include "common/Logging.h"

#include "Renderer.h"
#include "Device.h"

#include "VkHelper.h"

namespace maj::rhi
{
	RenderPass::Info::Info(std::vector<AttachmentInfo>&& attachments, std::vector<Subpass>&& subpasses, std::vector<SubpassDependency>&& dependencies)
		: mAttachments { std::move(attachments) }, mSubpasses { std::move(subpasses) }, mDependencies { std::move(dependencies) }
	{}

	RenderPass::Info::Info(Info&& other)
		: mAttachments { std::move(other.mAttachments) }, mSubpasses { std::move(other.mSubpasses) }, mDependencies { std::move(other.mDependencies) }
	{}

	RenderPass::Info& RenderPass::Info::operator=(Info&& other)
	{
		mAttachments = std::move(other.mAttachments);
		mSubpasses = std::move(other.mSubpasses);
		mDependencies = std::move(other.mDependencies);

		return *this;
	}

	RenderPass::Pass::Pass(const RenderPass& kRenderPass, VkFramebuffer framebuffer, const VkRect2D& kArea, VkCommandBuffer commandBuffer, VkSubpassContents subpassContents)
		: mInfo { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, nullptr, kRenderPass.mRenderPass, framebuffer, kArea, static_cast<uint32_t>(kRenderPass.mClearValues.size()), kRenderPass.mClearValues.data() },
		mCommandBuffer { commandBuffer }, mSubpassContents { subpassContents }, mRemainingSubpass { static_cast<uint32_t>(kRenderPass.mSubpasses.size()) }
	{
		if (!mCommandBuffer)
			return;

		vkCmdBeginRenderPass(mCommandBuffer, &mInfo, mSubpassContents);
	}

	RenderPass::Pass::~Pass()
	{
		if (!mCommandBuffer)
			return;

		for (uint32_t i { mRemainingSubpass } ; i > 1u ; --i)
			vkCmdNextSubpass(mCommandBuffer, mSubpassContents);

		vkCmdEndRenderPass(mCommandBuffer);
	}

	void RenderPass::Pass::next()
	{
		if (mRemainingSubpass <= 1u || !mCommandBuffer)
			return;

		vkCmdNextSubpass(mCommandBuffer, mSubpassContents);
		--mRemainingSubpass;
	}

	RenderPass::RenderPass(Info&& info)
		: mAttachments { std::move(info.mAttachments) }, mClearValues {}, mSubpasses { std::move(info.mSubpasses) }, mRenderPass { VK_NULL_HANDLE }
	{
		mClearValues.resize(mAttachments.size());
		createRenderPass(info.mDependencies);
	}

	RenderPass::RenderPass(RenderPass&& other)
		: mAttachments { std::move(other.mAttachments) }, mClearValues { std::move(other.mClearValues) }, mSubpasses { std::move(other.mSubpasses) },
		mRenderPass { other.mRenderPass }
	{
		other.mRenderPass = VK_NULL_HANDLE;
	}

	RenderPass::~RenderPass()
	{
		const Device* kDevice { Service::get<Device>() };

		if (mRenderPass)
		{
			Service::get<Renderer>()->frameGraph().remove(*this);
			vkDestroyRenderPass(kDevice->device(), mRenderPass, nullptr);
		}
	}

	const std::vector<AttachmentInfo>& RenderPass::attachments() const
	{
		return mAttachments;
	}

	uint32_t RenderPass::countAttachments(EAttachmentUsageFlag usage) const
	{
		uint32_t count { 0u };
		uint32_t attachmentCount { static_cast<uint32_t>(mAttachments.size()) };
		for (uint32_t i { 0u } ; i < attachmentCount ; ++i)
		{
			if (mAttachments[i].usage() != usage)
				continue;

			++count;
		}

		return count;
	}

	std::vector<AttachmentInfo> RenderPass::attachments(EAttachmentUsageFlag usage) const
	{
		std::vector<AttachmentInfo> attachments {};
		uint32_t attachmentCount { static_cast<uint32_t>(mAttachments.size()) };

		for (uint32_t i { 0u } ; i < attachmentCount ; ++i)
		{
			if (mAttachments[i].usage() != usage)
				continue;

			attachments.emplace_back(mAttachments[i]);
		}

		return attachments;
	}

	const std::vector<Subpass>& RenderPass::subpasses() const
	{
		return mSubpasses;
	}
	
	VkRenderPass RenderPass::renderPass() const
	{
		return mRenderPass;
	}

	void RenderPass::clearColor(uint32_t attachmentIndex, ClearColor&& clearColor)
	{
		const AttachmentInfo& kAttachment { mAttachments[attachmentIndex] };
		if (!kAttachment.hasEither(EAttachmentOpFlag::CLEAR_COLOR))
		{
			LOG(LogRHIWarning, "failed to set clear color: attachment " << attachmentIndex << " won't be cleared with color")
			return;
		}

		mClearValues[attachmentIndex].color = toClearColor(clearColor);
	}

	void RenderPass::clearDepthStencil(uint32_t attachmentIndex, ClearDepthStencil&& clearDepthStencil)
	{
		const AttachmentInfo& kAttachment { mAttachments[attachmentIndex] };
		if (!kAttachment.hasEither(EAttachmentOpFlag::CLEAR_DEPTH | EAttachmentOpFlag::CLEAR_STENCIL))
		{
			LOG(LogRHIWarning, "failed to set clear depth stencil: attachment " << attachmentIndex << " won't be cleared with depth and/or stencil")
			return;
		}

		mClearValues[attachmentIndex].depthStencil = toClearDepthStencil(clearDepthStencil);
	}

	RenderPass& RenderPass::operator=(RenderPass&& other)
	{
		const Device* kDevice { Service::get<Device>() };

		if (mRenderPass)
		{
			Service::get<Renderer>()->frameGraph().remove(*this);
			vkDestroyRenderPass(kDevice->device(), mRenderPass, nullptr);
		}

		mRenderPass = other.mRenderPass;
		other.mRenderPass = VK_NULL_HANDLE;

		mAttachments = std::move(other.mAttachments);
		mClearValues = std::move(other.mClearValues);
		mSubpasses = std::move(other.mSubpasses);

		return *this;
	}

	void RenderPass::createRenderPass(const std::vector<SubpassDependency>& kDependencies)
	{
		std::vector<VkAttachmentDescription> attachmentDescriptions { AttachmentInfo::descriptions(mAttachments) };
		std::vector<Subpass::Description> subpassDescriptions { Subpass::descriptions(mSubpasses) };

		std::vector<VkSubpassDescription> descriptions {};
		std::vector<VkSubpassDependency> dependencies {};
		uint32_t subpassCount { static_cast<uint32_t>(subpassDescriptions.size()) };
		uint32_t dependenciesCount { static_cast<uint32_t>(kDependencies.size()) };
		descriptions.reserve(subpassCount);
		dependencies.reserve(dependenciesCount);

		for (uint32_t i { 0u } ; i < subpassCount ; ++i)
			descriptions.emplace_back(subpassDescriptions[i].description());

		for (uint32_t i { 0u } ; i < dependenciesCount ; ++i)
			dependencies.emplace_back(toSubpassDependency(kDependencies[i]));

		VkRenderPassCreateInfo renderPassInfo { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
		renderPassInfo.pAttachments = attachmentDescriptions.data();
		renderPassInfo.subpassCount = subpassCount;
		renderPassInfo.pSubpasses = descriptions.data();
		renderPassInfo.dependencyCount = dependenciesCount;
		renderPassInfo.pDependencies = dependencies.data();

		VkResult result = vkCreateRenderPass(Service::get<Device>()->device(), &renderPassInfo, nullptr, &mRenderPass);
		if (result != VK_SUCCESS)
			THROW("failed to create render pass with error: " + std::to_string(result))
	}
}