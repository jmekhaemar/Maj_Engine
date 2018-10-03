#ifndef _SKELETON_H_
#define _SKELETON_H_

#include <vector>

#include "anim/Joint.h"

namespace maj::anim
{
	struct Skeleton final
	{
	public:
		Skeleton() = default;
		Skeleton(std::vector<Joint>&& joints);
		~Skeleton() = default;

		void addJoint(Joint&& joint);
		uint32_t jointCount() const;

		Joint& joint(uint32_t index);
		const Joint& joint(uint32_t index) const;

		void clear();

		uint32_t jointIndexFromName(const char* kName) const;

	private:
		std::vector<Joint> mJoints;
	};
}


#endif