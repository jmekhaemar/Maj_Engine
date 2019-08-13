#include "Framebuffer.h"

#include "Device.h"
#include "Renderer.h"

namespace maj::rhi
{
	Framebuffer::Framebuffer(const RenderPass& kRenderPass, const std::vector<const Image*>& kAttachments, const math::Vec2ui& kExtent)
		: mkExtent { kExtent }, mFramebuffer { VK_NULL_HANDLE }, mCommandBuffer { VK_NULL_HANDLE }
	{
		createFramebuffer(kRenderPass, kAttachments);
		createCommandBuffer();
	}

	Framebuffer::Framebuffer(Framebuffer&& other)
		: mkExtent { other.mkExtent }, mFramebuffer { other.mFramebuffer }, mCommandBuffer { other.mCommandBuffer }
	{
		other.mFramebuffer = VK_NULL_HANDLE;
		other.mCommandBuffer = VK_NULL_HANDLE;
	}

	Framebuffer::~Framebuffer()
	{
		const Device* kDevice { Service::get<Device>() };

		if (mCommandBuffer)
			vkFreeCommandBuffers(kDevice->device(), Service::get<Renderer>()->commandPool(), 1, &mCommandBuffer);

		if (mFramebuffer)
			vkDestroyFramebuffer(kDevice->device(), mFramebuffer, nullptr);
	}

	void Framebuffer::record(const RenderPass& kRenderPass, const FrameGraph& kFrameGraph) const
	{
		// Setup command buffer
		VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		beginInfo.flags = 0u;

		vkBeginCommandBuffer(mCommandBuffer, &beginInfo);

		// Render Pass
		{
			RenderPass::Pass pass { kRenderPass, mFramebuffer, {{0, 0}, {mkExtent.mX, mkExtent.mY}}, mCommandBuffer, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS };

			uint32_t subpassCount { static_cast<uint32_t>(kRenderPass.subpasses().size()) };
			for (uint32_t i { 0u } ; i < subpassCount ; ++i)
			{
				std::vector<VkCommandBuffer> secondaryBuffers { kFrameGraph.commandBuffers(kRenderPass, i) };

				uint32_t secondaryCount { static_cast<uint32_t>(secondaryBuffers.size()) };
				if (secondaryCount > 0u)
					vkCmdExecuteCommands(mCommandBuffer, secondaryCount, secondaryBuffers.data());

				pass.next();
			}
		}

		VkResult result{ vkEndCommandBuffer(mCommandBuffer) };
		if (result != VK_SUCCESS)
			THROW("failed to record command buffer with error: " + std::to_string(result))
	}

	void Framebuffer::submit(Fence* fence) const
	{
		const Device* kDevice { Service::get<Device>() };

		VkSubmitInfo submitInfo { VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.commandBufferCount = 1u;
		submitInfo.pCommandBuffers = &mCommandBuffer;

		if (fence)
			fence->reset();

		VkResult result { vkQueueSubmit(kDevice->queueHandler().queue("graphics"), 1u, &submitInfo, fence ? fence->fence() : VK_NULL_HANDLE) };
		if (result != VK_SUCCESS)
			THROW("failed to submit to graphics queue with error: " + std::to_string(result))

		if (fence)
			fence->validate();
	}

	VkCommandBuffer Framebuffer::commandBuffer() const
	{
		return mCommandBuffer;
	}

	void Framebuffer::createFramebuffer(const RenderPass& kRenderPass, const std::vector<const Image*>& kAttachments)
	{
		uint32_t attachmentCount { static_cast<uint32_t>(kAttachments.size()) };
		std::vector<VkImageView> views {};
		views.reserve(attachmentCount);

		for (const Image* kImage : kAttachments)
		{
			if (!kImage)
				THROW("failed to create framebuffer with error: attachment missing")

			views.emplace_back(kImage->view());
		}

		VkFramebufferCreateInfo framebufferInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
		framebufferInfo.renderPass = kRenderPass.renderPass();
		framebufferInfo.attachmentCount = attachmentCount;
		framebufferInfo.pAttachments = views.data();
		framebufferInfo.width = mkExtent.mX;
		framebufferInfo.height = mkExtent.mY;
		framebufferInfo.layers = 1u;

		VkResult result{ vkCreateFramebuffer(Service::get<Device>()->device(), &framebufferInfo, nullptr, &mFramebuffer) };
		if (result != VK_SUCCESS)
			THROW("failed to create framebuffer with error: " + std::to_string(result))
	}

	void Framebuffer::createCommandBuffer()
	{
		const Renderer* kRenderer{ Service::get<Renderer>() };

		VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.commandPool = kRenderer->commandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1u;

		VkResult result{ vkAllocateCommandBuffers(Service::get<Device>()->device(), &allocInfo, &mCommandBuffer) };
		if (result != VK_SUCCESS)
			THROW("failed to allocate command buffer with error: " + std::to_string(result))
	}
}