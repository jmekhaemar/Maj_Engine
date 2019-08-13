#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

#include "Device.h"
#include "AttachmentInfo.h"
#include "Subpass.h"

namespace maj::rhi
{
	class RenderPass final : public NonCopyable
	{
	public:
		struct Info final : public NonCopyable
		{
			Info(std::vector<AttachmentInfo>&& attachments, std::vector<Subpass>&& subpasses, std::vector<SubpassDependency>&& dependencies);
			Info(Info&& other);
			~Info() = default;

			Info& operator=(Info&& other);

			std::vector<AttachmentInfo> mAttachments;
			std::vector<Subpass> mSubpasses;
			std::vector<SubpassDependency> mDependencies;
		};

		struct Pass final
		{
		public:
			Pass(const RenderPass& kRenderPass, VkFramebuffer framebuffer, const VkRect2D& kArea, VkCommandBuffer commandBuffer, VkSubpassContents subpassContents);
			~Pass();

			void next();

		private:
			VkRenderPassBeginInfo mInfo;
			VkCommandBuffer mCommandBuffer;
			VkSubpassContents mSubpassContents;

			uint32_t mRemainingSubpass;
		};

	public:
		RenderPass(Info&& info);
		RenderPass(RenderPass&& other);
		~RenderPass();

		const std::vector<AttachmentInfo>& attachments() const;
		uint32_t countAttachments(EAttachmentUsageFlag usage) const;
		std::vector<AttachmentInfo> attachments(EAttachmentUsageFlag usage) const;

		const std::vector<Subpass>& subpasses() const;
		VkRenderPass renderPass() const;

		void clearColor(uint32_t attachmentIndex, ClearColor&& clearColor);
		void clearDepthStencil(uint32_t attachmentIndex, ClearDepthStencil&& clearDepthStencil);

		RenderPass& operator=(RenderPass&& other);

	private:
		void createRenderPass(const std::vector<SubpassDependency>& kDependencies);

		std::vector<AttachmentInfo> mAttachments;
		std::vector<VkClearValue> mClearValues;

		std::vector<Subpass> mSubpasses;
		VkRenderPass mRenderPass;
	};
}

#endif