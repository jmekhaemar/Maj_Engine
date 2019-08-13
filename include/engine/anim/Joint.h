#ifndef _JOINT_H_
#define _JOINT_H_

#include <vector>

#include <Transform.h>

namespace maj::anim
{
	struct Joint final
	{
	public:
		Joint(const char* kName, uint32_t nbChild = 0u);
		~Joint() = default;

		const char* mName;
		std::vector<uint32_t> mChildrenIndex;
		math::Transf mGlobalBind;
	};
}

#endif