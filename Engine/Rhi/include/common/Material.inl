#ifndef _RHI_MATERIAL_INL_
#define _RHI_MATERIAL_INL_

#include "common/Material.h"

namespace maj::rhi
{
	constexpr bool operator<(const MaterialBinding& kLhs, const MaterialBinding& kRhs)
	{
		if (kLhs.mSet == kRhs.mSet)
			return kLhs.mBinding < kRhs.mBinding;

		return kLhs.mSet < kRhs.mSet;
	}

	constexpr bool operator>(const MaterialBinding& kLhs, const MaterialBinding& kRhs)
	{
		if (kLhs.mSet == kRhs.mSet)
			return kLhs.mBinding > kRhs.mBinding;

		return kLhs.mSet > kRhs.mSet;
	}

	DEFINE_FLAG(ECullModeFlag, uint8_t)
	DEFINE_FLAG(EColorComponentFlag, uint8_t)
}

#endif