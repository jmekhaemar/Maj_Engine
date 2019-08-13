#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <Service.h>

#include "SwapChain.h"
#include "FrameGraph.h"
#include "Buffer.h"

namespace maj::rhi
{
	class Renderer final : public Service
	{
	public:
		struct SingleTimeCommand final : public NonCopyable
		{
			SingleTimeCommand();
			~SingleTimeCommand();

			VkCommandBuffer mBuffer;
		};

	private:
		struct CommandPool final : public NonCopyable
		{
			CommandPool();
			~CommandPool();

			VkCommandPool mPool;
		};

		struct Context final
		{
			Context(SwapChain::Info&& defaultSwapChain, RenderPass::Info&& defaultRenderPass);
			~Context() = default;

			Fence mSubmitFinished;
			std::unordered_map<uint8_t, SwapChain> mSwapChains;
		};

		static SwapChain::Info defaultSwapChain(uint8_t viewportIndex);
		static RenderPass::Info defaultRenderPass();

	public:
		Renderer();
		~Renderer();

		VkCommandPool commandPool() const;
		const SwapChain& swapChain(uint8_t i) const;

		void addSwapChain(uint8_t viewportIndex);
		void removeSwapChain(uint8_t viewportIndex);

		void wait() const;
		void draw();

		const FrameGraph& frameGraph() const;
		FrameGraph& frameGraph();

		Event<void> mOnDraw;

	private:
		void resize(uint8_t viewportIndex, const math::Vec2ui& kExtent);

		CommandPool mCommandPool;
		FrameGraph mFrameGraph;
		Context mContext;
		Buffer::Handler mBufferHandler;
	};
}

#endif