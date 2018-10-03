#include "Renderer.h"

#include <Window.h>

#include "Instance.h"
#include "VkHelper.h"

namespace maj::rhi
{
	Renderer::SingleTimeCommand::SingleTimeCommand()
	{
		const Device* kDevice { Service::get<Device>() };

		VkCommandBufferAllocateInfo allocInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.commandPool = Service::get<Renderer>()->mCommandPool.mPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1u;

		VkResult result { vkAllocateCommandBuffers(kDevice->device(), &allocInfo, &mBuffer) };
		if (result != VK_SUCCESS)
			THROW("failed to allocate command buffer with error: " + std::to_string(result))

		VkCommandBufferBeginInfo beginInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(mBuffer, &beginInfo);
	}

	Renderer::SingleTimeCommand::~SingleTimeCommand()
	{
		if (!mBuffer)
			return;

		const Device* kDevice { Service::get<Device>() };

		vkEndCommandBuffer(mBuffer);

		VkSubmitInfo submitInfo { VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.commandBufferCount = 1u;
		submitInfo.pCommandBuffers = &mBuffer;

		vkQueueSubmit(kDevice->queueHandler().queue("graphics"), 1u, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(kDevice->queueHandler().queue("graphics"));

		vkFreeCommandBuffers(kDevice->device(), Service::get<Renderer>()->mCommandPool.mPool, 1u, &mBuffer);
	}

	Renderer::CommandPool::CommandPool()
		: mPool { VK_NULL_HANDLE }
	{
		const Device* kDevice { Service::get<Device>() };

		VkCommandPoolCreateInfo poolInfo { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = kDevice->queueHandler().familyIndex("graphics");

		VkResult result { vkCreateCommandPool(kDevice->device(), &poolInfo, nullptr, &mPool) };
		if (result != VK_SUCCESS)
			THROW("failed to create command pool with error:" + std::to_string(result))
	}

	Renderer::CommandPool::~CommandPool()
	{
		const Device* kDevice { Service::get<Device>() };

		if (mPool)
			vkDestroyCommandPool(kDevice->device(), mPool, nullptr);
	}

	Renderer::Context::Context(SwapChain::Info&& defaultSwapChain, RenderPass::Info&& defaultRenderPass)
		: mSubmitFinished {}, mSwapChains {}
	{
		SwapChain swapChain { std::move(defaultSwapChain), std::move(defaultRenderPass) };
		mSwapChains.emplace(static_cast<uint8_t>(0u), std::move(swapChain));
	}

	SwapChain::Info Renderer::defaultSwapChain(uint8_t viewportIndex)
	{
		const Instance* kInstance { Service::get<Instance>() };
		const ww::Viewport& kViewport { Service::get<ww::Window>()->viewport(viewportIndex) };
		const math::Vec2ui& kWindowExtent { kViewport.extent() };

		return { kInstance->surface(viewportIndex), kWindowExtent };
	}

	RenderPass::Info Renderer::defaultRenderPass() // TODO move to config
	{
		const Device* kDevice { Service::get<Device>() };
		Format colorFormat { Format::EComponent::B8G8R8A8, Format::EType::UNORM };
		Format depthFormat { kDevice->findSupportedFormat({ { Format::EComponent::D32, Format::EType::SFLOAT }, { Format::EComponent::D32S8, Format::EType::SFLOAT },
															{ Format::EComponent::D24S8, Format::EType::UNORM } }, EImageTiling::OPTIMAL,
															EFormatFeatureFlag::DEPTH_STENCIL_ATTACHMENT) };
		
		RenderPass::Info renderPassInfo
		{
			// Attachments
			{
				// Attachment 0
				{
					EAttachmentUsageFlag::COLOR,
					{
						colorFormat, ESampleFlag::COUNT_1,
						EAttachmentOpFlag::CLEAR_COLOR | EAttachmentOpFlag::STORE_COLOR,
						EImageLayout::UNDEFINED, EImageLayout::PRESENT
					}
				},
				// Attachment 1
				{
					EAttachmentUsageFlag::DEPTH,
					{
						depthFormat, ESampleFlag::COUNT_1,
						EAttachmentOpFlag::CLEAR_DEPTH,
						EImageLayout::UNDEFINED, EImageLayout::DEPTH_STENCIL_ATTACHMENT
					}
				}
			},
			// Subpasses
			{
				// Subpass 0
				{
					{												// Color attachments
						{ 0u, EImageLayout::COLOR }
					},
					{},												// Input attachments
					{ 1u, EImageLayout::DEPTH_STENCIL_ATTACHMENT }	// Depth-stencil attachment
				}
			},
			// Dependencies
			{}
		};

		return renderPassInfo;
	}

	Renderer::Renderer()
		: Service { typeid(Renderer).name() }, mCommandPool {}, mFrameGraph {}, mContext { defaultSwapChain(0u), defaultRenderPass() }
	{
		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		viewport.mOnResized += std::bind(&Renderer::resize, this, std::placeholders::_1, std::placeholders::_2);

		mContext.mSwapChains.at(0u).renderPass().clearColor(0u, { 0.15f, 0.15f, 0.15f, 1.f });
		mContext.mSwapChains.at(0u).renderPass().clearDepthStencil(1u, { 1.f, 0u });
	}

	Renderer::~Renderer()
	{
		const Device* kDevice { Service::get<Device>() };
		vkDeviceWaitIdle(kDevice->device());
	}

	VkCommandPool Renderer::commandPool() const
	{
		return mCommandPool.mPool;
	}

	const SwapChain& Renderer::swapChain(uint8_t i) const
	{
		if (mContext.mSwapChains.count(i) == 0u)
		{
			LOG(LogRHIWarning, "failed to retrieve swap chain: unknown index")
			return mContext.mSwapChains.at(0u);
		}

		return mContext.mSwapChains.at(i);
	}

	void Renderer::addSwapChain(uint8_t viewportIndex)
	{
		SwapChain swapChain { defaultSwapChain(viewportIndex), defaultRenderPass() };
		mContext.mSwapChains.emplace(static_cast<uint8_t>(viewportIndex), std::move(swapChain));

		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(viewportIndex) };
		viewport.mOnResized += std::bind(&Renderer::resize, this, std::placeholders::_1, std::placeholders::_2);
	}

	void Renderer::removeSwapChain(uint8_t viewportIndex)
	{
		if (mContext.mSwapChains.count(viewportIndex) == 0u)
			return;

		mContext.mSubmitFinished.wait(1000u);
		mContext.mSwapChains.erase(viewportIndex);
	}
	
	void Renderer::wait() const
	{
		const Device* kDevice { Service::get<Device>() };
		vkQueueWaitIdle(kDevice->queueHandler().queue("graphics"));
	}

	void Renderer::draw()
	{
		mFrameGraph.record();

		std::unordered_map<uint8_t, bool> shouldResize { SwapChain::present(mContext.mSwapChains, &mContext.mSubmitFinished) };
		for (const std::pair<const uint8_t, bool>& kPair : shouldResize)
		{
			if (kPair.second)
			{
				const ww::Viewport& kViewport { Service::get<ww::Window>()->viewport(kPair.first) };
				resize(kPair.first, kViewport.extent());
			}
		}

		mOnDraw();
	}
	
	void Renderer::resize(uint8_t viewportIndex, const math::Vec2ui& kExtent)
	{
		mContext.mSwapChains.at(viewportIndex).resize(viewportIndex, kExtent);
		mFrameGraph.dirty(mContext.mSwapChains.at(viewportIndex).renderPass());
	}

	FrameGraph& Renderer::frameGraph()
	{
		return mFrameGraph;
	}

	const FrameGraph& Renderer::frameGraph()const
	{
		return mFrameGraph;
	}
}