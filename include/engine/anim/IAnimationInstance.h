#ifndef _IANIMATION_INSTANCE_H_
#define _IANIMATION_INSTANCE_H_

#include <Transform.h>

namespace maj::anim
{ 
	class IAnimationInstance
	{
	public:
		virtual ~IAnimationInstance() = default;

		virtual void update(float deltaTime) = 0;
		virtual math::Transf transform(uint32_t boneIndex) const = 0;
		virtual void playSpeed(float playSpeed) = 0;
	};
}


#endif