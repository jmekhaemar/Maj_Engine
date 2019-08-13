#ifndef _ANMATION_H_
#define _ANMATION_H_

#include <unordered_map>
#include <vector>
#include <string>

#include "anim/Keyframe.h"
#include "anim/Skeleton.h"

namespace maj::anim
{ 
	class Animation final
	{
	public:
		Animation(const std::string& kStr);
		Animation(const std::string& kStr, std::vector<Keyframe>&& frames);
		~Animation() = default;

		math::Transf transform(float keyIndex, uint32_t boneIndex) const;

		const std::string& name() const;
		uint32_t keyCount()const;

	protected:
		std::string mName;
		std::vector<Keyframe> mKeyframes;
	};
}


#endif