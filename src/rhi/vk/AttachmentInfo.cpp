#include "AttachmentInfo.h"

#include "common/Logging.h"
#include "VkHelper.h"

namespace maj::rhi
{
	std::vector<VkAttachmentDescription> AttachmentInfo::descriptions(const std::vector<AttachmentInfo>& kAttachments)
	{
		uint32_t attachmentCount {static_cast<uint32_t>(kAttachments.size())};
		if (attachmentCount == 0u)
			return {};

		std::vector<VkAttachmentDescription> descriptions {};
		descriptions.reserve(attachmentCount);
		for (uint32_t i = 0u ; i < attachmentCount ; ++i)
			descriptions.emplace_back(toAttachmentDescription(kAttachments[i].mDescription));

		return descriptions;
	}

	AttachmentInfo::AttachmentInfo(EAttachmentUsageFlag usage, AttachmentDescription&& description)
		: mUsage { usage }, mDescription { description }
	{}

	bool AttachmentInfo::hasEither(EAttachmentOpFlag flags) const
	{
		return (mDescription.mOp & flags) != EAttachmentOpFlag::DONT_CARE;
	}

	EAttachmentUsageFlag AttachmentInfo::usage() const
	{
		return mUsage;
	}

	const AttachmentDescription& AttachmentInfo::description() const
	{
		return mDescription;
	}
}