#include "anim/VertexBlendingInfo.h"

namespace maj::anim
{
	VertexBlendingInfo::VertexBlendingInfo(float blendingIndex, float blendingWeight)
		:mBlendingIndex{ blendingIndex }, mBlendingWeight{ blendingWeight }
	{}

	bool VertexBlendingInfo::operator<(const VertexBlendingInfo& rhs)
	{
		return (mBlendingWeight > rhs.mBlendingWeight);
	}
}