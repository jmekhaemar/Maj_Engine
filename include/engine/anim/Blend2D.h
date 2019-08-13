#ifndef _BLEND2D_H_
#define _BLEND2D_H_

#include <functional>

#include <Transform.h>

#include "anim/IAnimationInstance.h"
#include "anim/AnimationInstance.h"

namespace maj::anim
{ 
	class Blend2D final : public IAnimationInstance
	{
	public:
		Blend2D(const Animation& kStartAnim, const Animation& kEndAnim, std::function<float()>&& weight);
		Blend2D(const Animation& kStartAnim, const Animation& kEndAnim);
		virtual ~Blend2D() = default;

		virtual void update(float deltaTime) override;
		virtual math::Transf transform(uint32_t boneIndex) const override;
		virtual void playSpeed(float playSpeed) override;

	private:
		AnimationInstance mStart;
		AnimationInstance mEnd;

		std::function<float()> mWeightFunc;
		float mWeight;
	};
}

#endif