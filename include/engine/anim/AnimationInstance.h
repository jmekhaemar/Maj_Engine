#ifndef _ANMATION_INSTANCE_H_
#define _ANMATION_INSTANCE_H_

#include <string>

#include "anim/Animation.h"
#include "anim/IAnimationInstance.h"

namespace maj::anim
{
	class AnimationInstance final : public IAnimationInstance
	{
	public:
		AnimationInstance(const Animation& kAnim);
		~AnimationInstance() = default;

		virtual void update(float deltaTime) override;
		virtual math::Transf transform(uint32_t boneIndex) const override;
		virtual void playSpeed(float playSpeed)override;

		float time()const;
		void time(float time);

		float keyFrame()const;

	protected:
		const Animation& mkAnim;

		float mTime;
		float mKeyFrame;
		float mPlaySpeed;
	};
}

#endif