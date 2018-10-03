#include "anim/Animation.h"

namespace maj::anim
{
	Animation::Animation(const std::string& kStr)
		: mName { kStr }, mKeyframes {}
	{}
	
	Animation::Animation(const std::string& kStr, std::vector<Keyframe>&& frames)
		: mName { kStr }, mKeyframes { std::move(frames) }
	{}

	math::Transf Animation::transform(float keyIndex, uint32_t boneIndex) const
	{
		uint32_t index { static_cast<uint32_t>(keyIndex) };

		uint32_t keyCount { static_cast<uint32_t>(mKeyframes.size()) };
		return math::Transf::lerp(mKeyframes[index].transform(boneIndex),
								  mKeyframes[(index + 1u) % keyCount].transform(boneIndex),
								  keyIndex - static_cast<float>(index));
	}

	const std::string& Animation::name()const
	{
		return mName;
	}

	uint32_t Animation::keyCount()const
	{
		return static_cast<uint32_t>(mKeyframes.size());
	}
}