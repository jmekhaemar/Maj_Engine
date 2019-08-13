#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <vulkan/vulkan.h>
#include <vector>

#include <Vec2.h>
#include <NonCopyable.h>

#include "RenderPass.h"
#include "FrameGraph.h"

namespace maj::rhi
{
	class Image;
	class Fence;

	class Framebuffer final : public NonCopyable
	{
	public:
		Framebuffer(const RenderPass& kRenderPass, const std::vector<const Image*>& kAttachments, const math::Vec2ui& kExtent);
		Framebuffer(Framebuffer&& other);
		~Framebuffer();

		void record(const RenderPass& kRenderPass, const FrameGraph& kFrameGraph) const;
		void submit(Fence* fence) const;

		VkCommandBuffer commandBuffer() const;

	private:
		void createFramebuffer(const RenderPass& kRenderPass, const std::vector<const Image*>& kAttachments);
		void createCommandBuffer();

		const math::Vec2ui mkExtent;

		VkFramebuffer mFramebuffer;
		VkCommandBuffer mCommandBuffer;
	};
}

#endif