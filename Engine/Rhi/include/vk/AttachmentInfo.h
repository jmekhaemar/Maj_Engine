#ifndef _ATTACHMENT_INFO_H_
#define _ATTACHMENT_INFO_H_

#include <vulkan/vulkan.h>
#include <vector>

#include "common/AttachmentDescription.h"

namespace maj::rhi
{
	class AttachmentInfo final
	{
	public:
		static std::vector<VkAttachmentDescription> descriptions(const std::vector<AttachmentInfo>& kAttachments);

	public:
		AttachmentInfo(EAttachmentUsageFlag usage, AttachmentDescription&& description);
		~AttachmentInfo() = default;

		bool hasEither(EAttachmentOpFlag flags) const;

		EAttachmentUsageFlag usage() const;
		const AttachmentDescription& description() const;

	private:
		EAttachmentUsageFlag mUsage;
		AttachmentDescription mDescription;
	};
}

#endif