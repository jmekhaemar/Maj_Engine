#ifndef _SWAP_CHAIN_H_
#define _SWAP_CHAIN_H_

#include "Surface.h"
#include "RenderPass.h"
#include "InputAttachment.h"
#include "Framebuffer.h"

namespace maj::rhi
{
	class SwapChain final : public NonCopyable
	{
	public:
		struct Info final
		{
		public:
			Info(const Surface& kSurface, const math::Vec2ui& kExtent);
			Info(Info&& other);
			~Info() = default;

			void create(const Surface& kSurface, const math::Vec2ui& kExtent);

			Info& operator=(Info&& other);

			VkSwapchainCreateInfoKHR mInfo;
		};
		
	private:
		struct Semaphore final : public NonCopyable
		{
		public:
			Semaphore();
			Semaphore(Semaphore&& other);
			~Semaphore();

			Semaphore& operator=(Semaphore&& other);

			VkSemaphore mSemaphore;
		};

	public:
		static std::unordered_map<uint8_t, bool> present(std::unordered_map<uint8_t, SwapChain>& swapChains, Fence* submitFinished);

	public:
		SwapChain(Info&& info, RenderPass::Info&& renderPassInfo);
		SwapChain(SwapChain&& other);
		~SwapChain();
		
		void resize(uint8_t viewportIndex, const math::Vec2ui& kExtent);

		RenderPass& renderPass();
		const RenderPass& renderPass() const;
		const InputAttachment* inputAttachment(EAttachmentUsageFlag usage, uint32_t index) const;

		SwapChain& operator=(SwapChain&& other);

	private:
		void createSwapChain();
		Image::ImgInfo createImageInfo(const AttachmentInfo& kAttachment) const;
		void createImages();
		void createFrames();

		void record(const FrameGraph& kFrameGraph) const;

		Info mInfo;
		RenderPass mRenderPass;

		VkSwapchainKHR mSwapChain;
		Semaphore mImageAvailable;
		Semaphore mRenderFinished;

		std::unordered_map<EAttachmentUsageFlag, std::vector<Image>> mImages;
		std::unordered_map<EAttachmentUsageFlag, std::vector<InputAttachment>> mInputAttachments;
		std::vector<Framebuffer> mFrames;

		uint32_t mRecordEventIndex;
	};
}

#endif