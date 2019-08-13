#include "anim/AnimationInstance.h"

#include "anim/Keyframe.h"

namespace maj::anim
{
	AnimationInstance::AnimationInstance(const Animation& kAnim)
		:mkAnim{ kAnim }, mTime{ 0.0f }, mKeyFrame{ 0.0f }, mPlaySpeed{ 1.f }
	{}

	void AnimationInstance::update(float deltaTime)
	{
		mTime += deltaTime * mPlaySpeed;

		while (mTime > 1.f)
			mTime -= 1.f;

		while(mTime < 0.f)
			mTime += 1.f;

		mKeyFrame = mTime * static_cast<float>(mkAnim.keyCount() - 1);
	}

	math::Transf AnimationInstance::transform(uint32_t boneIndex) const
	{
		return mkAnim.transform(mKeyFrame, boneIndex);
	}

	void AnimationInstance::playSpeed(float playSpeed)
	{
		mPlaySpeed = playSpeed;
	}

	float AnimationInstance::keyFrame()const
	{
		return mKeyFrame;
	}
}