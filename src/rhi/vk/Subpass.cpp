#include "Subpass.h"

#include "VkHelper.h"

namespace maj::rhi
{
	const uint32_t Subpass::skExternalIndex { VK_SUBPASS_EXTERNAL };

	Subpass::Description::Description(const Subpass& kSubpass)
		: mColorAttachments {}, mInputAttachments {}, mDepthStencilAttachment {}
	{
		uint32_t colorRefCount { static_cast<uint32_t>(kSubpass.mColorAttachments.size()) };
		mColorAttachments.reserve(colorRefCount);
		for (uint32_t i { 0u } ; i < colorRefCount ; ++i)
			mColorAttachments.emplace_back(toAttachmentRef(kSubpass.mColorAttachments[i]));

		uint32_t inputRefCount { static_cast<uint32_t>(kSubpass.mInputAttachments.size()) };
		mInputAttachments.reserve(inputRefCount);
		for (uint32_t i { 0u } ; i < inputRefCount ; ++i)
			mInputAttachments.emplace_back(toAttachmentRef(kSubpass.mInputAttachments[i]));

		if (kSubpass.mDepthStencilAttachment)
			mDepthStencilAttachment = new VkAttachmentReference { toAttachmentRef(*kSubpass.mDepthStencilAttachment) };
	}

	VkSubpassDescription Subpass::Description::description() const
	{
		VkSubpassDescription description {};
		description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		description.colorAttachmentCount = static_cast<uint32_t>(mColorAttachments.size());
		description.pColorAttachments = mColorAttachments.data();
		description.inputAttachmentCount = static_cast<uint32_t>(mInputAttachments.size());
		description.pInputAttachments = mInputAttachments.data();
		description.pDepthStencilAttachment = mDepthStencilAttachment;

		return description;
	}

	std::vector<Subpass::Description> Subpass::descriptions(const std::vector<Subpass>& kSubpasses)
	{
		std::vector<Subpass::Description> descriptions {};
		uint32_t subpassCount { static_cast<uint32_t>(kSubpasses.size()) };
		descriptions.reserve(subpassCount);

		for (uint32_t i { 0u } ; i < subpassCount ; ++i)
			descriptions.emplace_back(kSubpasses[i]);

		return descriptions;
	}

	Subpass::Subpass(std::vector<AttachmentRef>&& colorAttachments, std::vector<AttachmentRef>&& inputAttachments)
		: mColorAttachments { std::move(colorAttachments) }, mInputAttachments { std::move(inputAttachments) }, mDepthStencilAttachment { nullptr }
	{}

	Subpass::Subpass(std::vector<AttachmentRef>&& colorAttachments, std::vector<AttachmentRef>&& inputAttachments, const AttachmentRef& depthStencilAttachment)
		: mColorAttachments { std::move(colorAttachments) }, mInputAttachments { std::move(inputAttachments) },
		mDepthStencilAttachment { new AttachmentRef {depthStencilAttachment} }
	{}

	Subpass::Subpass(const Subpass& kOther)
		: mColorAttachments { kOther.mColorAttachments }, mInputAttachments { kOther.mInputAttachments }, mDepthStencilAttachment { nullptr }
	{
		if (kOther.mDepthStencilAttachment)
			mDepthStencilAttachment = new AttachmentRef { *kOther.mDepthStencilAttachment };
	}

	Subpass::Subpass(Subpass&& other)
		: mColorAttachments { std::move(other.mColorAttachments) }, mInputAttachments { std::move(other.mInputAttachments) },
		mDepthStencilAttachment { other.mDepthStencilAttachment }
	{
		other.mDepthStencilAttachment = nullptr;
	}
	
	Subpass::~Subpass()
	{
		if (mDepthStencilAttachment)
			delete mDepthStencilAttachment;
	}

	const std::vector<AttachmentRef>& Subpass::colorAttachments() const
	{
		return mColorAttachments;
	}

	const std::vector<AttachmentRef>& Subpass::inputAttachments() const
	{
		return mInputAttachments;
	}

	const AttachmentRef* Subpass::depthStencilAttachment() const
	{
		return mDepthStencilAttachment;
	}

	Subpass& Subpass::operator=(Subpass&& other)
	{
		mColorAttachments = std::move(other.mColorAttachments);
		mInputAttachments = std::move(other.mInputAttachments);

		if (mDepthStencilAttachment && mDepthStencilAttachment != other.mDepthStencilAttachment)
			delete mDepthStencilAttachment;

		mDepthStencilAttachment = other.mDepthStencilAttachment;
		other.mDepthStencilAttachment = nullptr;

		return *this;
	}
}