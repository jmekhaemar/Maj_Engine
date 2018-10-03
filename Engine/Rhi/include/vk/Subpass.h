#ifndef _SUBPASS_H_
#define _SUBPASS_H_

#include <vulkan/vulkan.h>
#include <vector>

#include "common/AttachmentDescription.h"
#include "common/Pipeline.h"

namespace maj::rhi
{
	class Subpass
	{
	public:
		struct Description final
		{
			Description(const Subpass& kSubpass);
			~Description() = default;

			VkSubpassDescription description() const;

			std::vector<VkAttachmentReference> mColorAttachments;
			std::vector<VkAttachmentReference> mInputAttachments;
			VkAttachmentReference* mDepthStencilAttachment;
		};
		
		static std::vector<Description> descriptions(const std::vector<Subpass>& kSubpasses);

		static const uint32_t skExternalIndex;

	public:
		Subpass(std::vector<AttachmentRef>&& colorAttachments, std::vector<AttachmentRef>&& inputAttachments);
		Subpass(std::vector<AttachmentRef>&& colorAttachments, std::vector<AttachmentRef>&& inputAttachments,
				const AttachmentRef& depthStencilAttachment);
		Subpass(const Subpass& kOther);
		Subpass(Subpass&& other);
		~Subpass();

		const std::vector<AttachmentRef>& colorAttachments() const;
		const std::vector<AttachmentRef>& inputAttachments() const;
		const AttachmentRef* depthStencilAttachment() const;

		Subpass& operator=(Subpass&& other);

	private:
		std::vector<AttachmentRef> mColorAttachments;
		std::vector<AttachmentRef> mInputAttachments;
		AttachmentRef* mDepthStencilAttachment;
	};
}

#endif