#include "gizmo/ObjectPicker.h"

#include <Window.h>
#include <Renderer.h>

namespace maj::gizmo
{
	rhi::RenderPass::Info ObjectPicker::renderPassInfo()
	{
		const rhi::Device* kDevice { Service::get<rhi::Device>() };
		rhi::Format colorFormat { rhi::Format::EComponent::R8G8B8A8, rhi::Format::EType::UNORM };
		rhi::Format depthFormat { kDevice->findSupportedFormat({	{ rhi::Format::EComponent::D32, rhi::Format::EType::SFLOAT },
																	{ rhi::Format::EComponent::D32S8, rhi::Format::EType::SFLOAT },
																	{ rhi::Format::EComponent::D24S8, rhi::Format::EType::UNORM } },
																rhi::EImageTiling::OPTIMAL, rhi::EFormatFeatureFlag::DEPTH_STENCIL_ATTACHMENT) };

		rhi::RenderPass::Info info	{
										{	 // Attachments
											{	// Attachment 0
												rhi::EAttachmentUsageFlag::COLOR,
												{
													colorFormat, rhi::ESampleFlag::COUNT_1,
													rhi::EAttachmentOpFlag::CLEAR_COLOR | rhi::EAttachmentOpFlag::STORE_COLOR,
													rhi::EImageLayout::UNDEFINED, rhi::EImageLayout::TRANSFER_SRC
												}
											},
											{	// Attachment 1
												rhi::EAttachmentUsageFlag::DEPTH,
												{
													depthFormat, rhi::ESampleFlag::COUNT_1,
													rhi::EAttachmentOpFlag::CLEAR_DEPTH,
													rhi::EImageLayout::UNDEFINED, rhi::EImageLayout::DEPTH_STENCIL_ATTACHMENT
												}
											}
										},
										{	// Subpasses
											{	// Subpass 0
												{													// Color attachments
													{ 0u, rhi::EImageLayout::COLOR }
												},
												{},													// Input attachments
												{ 1u, rhi::EImageLayout::DEPTH_STENCIL_ATTACHMENT }	// Depth-stencil attachment
											}
										},
										{	// Dependencies
											{	// Dependency 0
												{ rhi::Subpass::skExternalIndex, rhi::EPipelineStageFlag::BOTTOM, rhi::EAccessFlag::MEMORY_READ },
												{ 0u, rhi::EPipelineStageFlag::COLOR_OUTPUT, rhi::EAccessFlag::COLOR_READ | rhi::EAccessFlag::COLOR_WRITE },
												true
											},
											{	// Dependency 1
												{ 0u, rhi::EPipelineStageFlag::COLOR_OUTPUT, rhi::EAccessFlag::COLOR_READ | rhi::EAccessFlag::COLOR_WRITE },
												{ rhi::Subpass::skExternalIndex, rhi::EPipelineStageFlag::BOTTOM, rhi::EAccessFlag::MEMORY_READ },
												true
											}
										}
									};
		return info;
	}

	ObjectPicker::ObjectPicker()
		: mRenderPass { renderPassInfo() }, mSubmitFinished {}, mImages {}, mFramebuffer {}, mPicker {}, mRecordEvent { 0u }, mResizeEvent { 0u }, mDrawEvent { 0u }
	{
		mRenderPass.clearColor(0u, { 0.f, 0.f, 0.f, 0.f });
		mRenderPass.clearDepthStencil(1u, { 1.f, 0u });

		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		const math::Vec2ui& kExtent { viewport.extent() };
		createImages(kExtent);
		createFramebuffer(kExtent);

		// Attach events
		rhi::Renderer* renderer { Service::get<rhi::Renderer>() };
		mDrawEvent = renderer->mOnDraw + [this] () -> void
		{
			if (mFramebuffer)
				mFramebuffer->submit(&mSubmitFinished);
		};

		rhi::FrameGraph& frameGraph { renderer->frameGraph() };
		mRecordEvent = frameGraph.mOnRecorded + [this, &frameGraph] () -> void
		{
			if (!mFramebuffer)
				return;

			mSubmitFinished.wait(1000u);
			mFramebuffer->record(mRenderPass, frameGraph);
		};
		mFramebuffer->record(mRenderPass, frameGraph);

		mResizeEvent = viewport.mOnResized + [this] (uint32_t id, const math::Vec2ui& kExtent) -> void
		{
			(void) id;
			resize(kExtent);
		};
	}

	ObjectPicker::~ObjectPicker()
	{
		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		viewport.mOnResized -= mResizeEvent;

		rhi::Renderer* renderer { Service::get<rhi::Renderer>() };
		renderer->mOnDraw -= mDrawEvent;

		rhi::FrameGraph& frameGraph { renderer->frameGraph() };
		frameGraph.mOnRecorded -= mRecordEvent;

		mSubmitFinished.wait(1000u);
	}

	uint32_t ObjectPicker::pick(const math::Vec2ui& kPosition)
	{
		if (mImages.size() < 1u)
			return 0u;

		// Create host visible image
		if (!mPicker)
		{
			rhi::Image::ImgInfo info { mImages[0u].info() };
			info.mLayout = rhi::EImageLayout::UNDEFINED;
			info.mTiling = rhi::EImageTiling::LINEAR;
			info.mUsage = rhi::EImageUsageFlag::TRANSFER_DST | rhi::EImageUsageFlag::COLOR;
			info.mMemory = rhi::EMemoryFlag::HOST_VISIBLE | rhi::EMemoryFlag::HOST_COHERENT;

			mPicker = std::make_unique<rhi::Image>(info);
		}

		// Copy image from framebuffer
		mSubmitFinished.wait(1000u);
		rhi::Image::copy(mImages[0u], *mPicker, rhi::EImageLayout::GENERAL);
		
		// Map memory & check pixel
		rhi::Image::MemoryMap memMap { *mPicker };

		uint32_t id { 0u };
		uint32_t coord = kPosition.mX * sizeof(uint32_t) + kPosition.mY * memMap.mRowStride;
		std::memcpy(&id, memMap.mData + coord, sizeof(uint32_t));

		return id;
	}

	const rhi::RenderPass& ObjectPicker::renderPass() const
	{
		return mRenderPass;
	}

	void ObjectPicker::createImages(const math::Vec2ui& kExtent)
	{
		const std::vector<rhi::AttachmentInfo>& kAttachments { mRenderPass.attachments() };
		std::vector<rhi::EImageLayout> layouts {};
		layouts.reserve(kAttachments.size());

		for (const rhi::AttachmentInfo& kAttachment : kAttachments)
		{
			const rhi::AttachmentDescription& kDescription { kAttachment.description() };

			rhi::Image::ImgInfo info {};
			info.mFormatComponent = kDescription.mFormat.mComponent;
			info.mFormatType = kDescription.mFormat.mType;
			info.mExtent = { kExtent.mX, kExtent.mY, 1u };
			info.mSamples = kDescription.mSamples;
			info.mUsage = rhi::toImageUsage(kAttachment.usage());
			info.mLayout = kDescription.mInitialLayout;

			if ((info.mUsage & rhi::EImageUsageFlag::COLOR) == rhi::EImageUsageFlag::COLOR)
				info.mUsage |= rhi::EImageUsageFlag::TRANSFER_SRC;

			mImages.emplace_back(info);
			layouts.emplace_back(kDescription.mFinalLayout);
		}

		rhi::Image::transitionLayout(mImages, layouts);
	}

	void ObjectPicker::createFramebuffer(const math::Vec2ui& kExtent)
	{
		std::vector<const rhi::Image*> images {};
		images.reserve(mImages.size());
		for (const rhi::Image& kImage : mImages)
			images.emplace_back(&kImage);

		mFramebuffer = std::make_unique<rhi::Framebuffer>(mRenderPass, images, kExtent);
	}

	void ObjectPicker::resize(const math::Vec2ui& kExtent)
	{
		mPicker.reset();
		mImages.clear();
		mFramebuffer.reset();

		createImages(kExtent);
		createFramebuffer(kExtent);

		Service::get<rhi::Renderer>()->frameGraph().dirty(mRenderPass);
	}
}