#ifndef _ATTACHMENT_DESCRIPTION_H_
#define _ATTACHMENT_DESCRIPTION_H_

#include <Flag.h>

#include "common/Format.h"
#include "common/Image.h"

namespace maj::rhi
{
	enum class EAttachmentOpFlag : uint8_t
	{
		DONT_CARE = 0u,
		LOAD_COLOR = 1u,
		CLEAR_COLOR = 2u,
		STORE_COLOR = 4u,
		LOAD_DEPTH = LOAD_COLOR,
		CLEAR_DEPTH = CLEAR_COLOR,
		STORE_DEPTH = STORE_COLOR,
		LOAD_STENCIL = 16u,
		CLEAR_STENCIL = 32u,
		STORE_STENCIL = 64u,
	};

	enum class EAttachmentUsageFlag : uint8_t
	{
		COLOR = 1u,
		DEPTH = 2u,
		STENCIL = 4u,
		INPUT = 8u
	};

	struct AttachmentDescription final
	{
		Format mFormat;
		ESampleFlag mSamples;
		EAttachmentOpFlag mOp;
		EImageLayout mInitialLayout;
		EImageLayout mFinalLayout;
	};

	struct AttachmentRef final
	{
		uint32_t mIndex;
		EImageLayout mLayout;
	};

	DECLARE_FLAG(EAttachmentOpFlag)
	DECLARE_FLAG(EAttachmentUsageFlag)

	constexpr EImageUsageFlag toImageUsage(EAttachmentUsageFlag usage) noexcept;
}

#include "common/AttachmentDescription.inl"

#endif