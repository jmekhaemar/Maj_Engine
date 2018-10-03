#include "anim/Blend2D.h"

namespace maj::anim
{
	Blend2D::Blend2D(const Animation& kStartAnim, const Animation& kEndAnim, std::function<float()>&& weightFunc)
		: mStart{ kStartAnim }, mEnd{ kEndAnim }, mWeightFunc{ weightFunc }, mWeight{ 0.f }
	{
	}

	Blend2D::Blend2D(const Animation& kStartAnim, const Animation& kEndAnim)
		: mStart{ kStartAnim }, mEnd{ kEndAnim }, mWeightFunc{ nullptr }, mWeight{ 0.f }
	{
	}

	void Blend2D::update(float deltaTime)
	{
		mStart.update(deltaTime);
		mEnd.update(deltaTime);
		mWeight = 0.0f;
	}

	math::Transf Blend2D::transform(uint32_t boneIndex) const
	{
		return math::Transf::lerp(mStart.transform(boneIndex), mEnd.transform(boneIndex), mWeight);
	}

	void Blend2D::playSpeed(float playSpeed)
	{
		mStart.playSpeed(playSpeed);
		mEnd.playSpeed(playSpeed);
	}
}