#ifndef _ATTACHMENT_DESCRIPTION_INL_
#define _ATTACHMENT_DESCRIPTION_INL_

#include "common/AttachmentDescription.h"

namespace maj::rhi
{
	DEFINE_FLAG(EAttachmentOpFlag, uint8_t)
	DEFINE_FLAG(EAttachmentUsageFlag, uint8_t)

	constexpr EImageUsageFlag toImageUsage(EAttachmentUsageFlag usage) noexcept
	{
		EImageUsageFlag flag { EImageUsageFlag::ATTACHMENT };
		if ((usage & EAttachmentUsageFlag::COLOR) == EAttachmentUsageFlag::COLOR)
			flag |= EImageUsageFlag::COLOR;

		if ((usage & EAttachmentUsageFlag::DEPTH) == EAttachmentUsageFlag::DEPTH)
			flag |= EImageUsageFlag::DEPTH;

		if ((usage & EAttachmentUsageFlag::STENCIL) == EAttachmentUsageFlag::STENCIL)
			flag |= EImageUsageFlag::STENCIL;

		if ((usage & EAttachmentUsageFlag::INPUT) == EAttachmentUsageFlag::INPUT)
			flag |= EImageUsageFlag::INPUT | EImageUsageFlag::SAMPLED;

		return flag;
	}
}

#endif