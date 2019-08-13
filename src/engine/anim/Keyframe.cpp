#include "anim/Keyframe.h"

#include <Logging.h>

namespace maj::anim
{
	const math::Transf& Keyframe::transform(uint32_t boneIndex) const
	{
		uint32_t count { static_cast<uint32_t>(mTransforms.size()) };
		if (boneIndex >= count)
			THROW("failed to get keyframe transform: bone index out of range")

		return mTransforms[boneIndex];
	}

	const std::vector<math::Transf>& Keyframe::transforms() const
	{
		return mTransforms;
	}

	void Keyframe::addTransform(math::Transf&& mat)
	{
		mTransforms.emplace_back(mat);
	}
}